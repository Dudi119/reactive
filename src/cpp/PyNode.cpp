#include "PyNode.h"
#include "sweetPy/CPythonObject.h"
#include "sweetPy/Core/Lock.h"
#include "sweetPy/Core/PythonAssist.h"
#include "core/Exception.h"
#include "GraphEngine.h"
#include "InputAdapter.h"

namespace reactive{
    PyFunctionSignature::~PyFunctionSignature()
    {
        for(auto& paramDescriptor : m_parametersDescriptors)
        {
            Py_DECREF(std::get<Type>(paramDescriptor));
            Py_DECREF(std::get<Value>(paramDescriptor));
        }
    }

    const PyFunctionSignature::Params& PyFunctionSignature::GetParams() const
    {
        return m_parametersDescriptors;
    }

    void PyFunctionSignature::AddParameter(ParameterUpdate parameterUpdate, PyObject* parameterType, PyObject *value)
    {
        Py_XINCREF(parameterType);
        Py_XINCREF(value);
        m_parametersDescriptors.emplace_back(std::make_tuple(parameterUpdate, parameterType, value));
    }

    PyNode::PyNode(sweetPy::object_ptr &&pyFunction, const PyFunctionSignature &signature, const PyNodeEdgesMetaData& meta)
        :m_pyFunction(std::move(pyFunction))
    {
        static LogicalId logicalId = 0;
        auto& params = signature.GetParams();
        for(int paramId = 0; paramId <  params.size(); paramId++)
        {
            if(std::get<PyFunctionSignature::Update>(params[paramId]) == PyFunctionSignature::Edge)
            {
                InputAdapter& incomingEdge = sweetPy::Object<InputAdapter&>::FromPython(std::get<PyFunctionSignature::Value>(params[paramId]));
                EdgeId inputId = incomingEdge.GetId();
                if(m_inEdgesMapping.find(inputId) != m_inEdgesMapping.end())
                    throw core::Exception(__CORE_SOURCE, "Input edge already exists - %d", inputId);
                m_inEdgesMapping[inputId] = logicalId++;
                m_inEdges.emplace_back(incomingEdge);
                m_arguments.AddElement(paramId, reinterpret_cast<void*>(&incomingEdge), [](void const * const ptr)->PyObject*{
                    const InputAdapter& inputAdapter = *reinterpret_cast<InputAdapter const *>(ptr);
                    return inputAdapter.GetLastData().release();
                });
            }
            else //Scalar
            {
                PyObject* scalar = std::get<PyFunctionSignature::Value>(params[paramId]);
                m_arguments.AddElement(paramId, sweetPy::object_ptr(scalar, &sweetPy::Deleter::Borrow));
            }
        }

        for(EdgeId id : meta.GetOutEdgesIds())
           m_outEdges.emplace(id, id);
    }

    PyNode::~PyNode() {}

    void PyNode::Invoke()
    {
        sweetPy::GilLock lock;
        sweetPy::Python::InvokeFunction(m_pyFunction, m_arguments);
    }

    void PyNode::PostStop()
    {
        sweetPy::GilLock lock;
        m_pyFunction.reset();
    }

    UnitNode& PyNodeFactory::Create(PyObject *pyFunction, const PyFunctionSignature &signature, const PyNodeEdgesMetaData& meta)
    {
        Py_XINCREF(pyFunction);
        std::unique_ptr<GraphNode> node(new PyNode(sweetPy::object_ptr(pyFunction, &sweetPy::Deleter::Owner), signature, meta));
        UnitNode& nodeRef = static_cast<UnitNode&>(*node);
        GraphEngine::Instance().RegisterNode(std::move(node));
        return nodeRef;
    }
}
