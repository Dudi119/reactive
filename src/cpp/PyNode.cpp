#include "PyNode.h"
#include "sweetPy/CPythonObject.h"
#include "sweetPy/Core/Lock.h"
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
        for(auto& paramDescriptor : signature.GetParams())
        {
            if(std::get<PyFunctionSignature::Update>(paramDescriptor) == PyFunctionSignature::Edge)
            {
                InputAdapter& incomingEdge = sweetPy::Object<InputAdapter&>::FromPython(std::get<PyFunctionSignature::Value>(paramDescriptor));
                EdgeId inputId = incomingEdge.GetId();
                if(m_inEdgesMapping.find(inputId) != m_inEdgesMapping.end())
                    throw core::Exception(__CORE_SOURCE, "Input edge already exists - %d", inputId);
                m_inEdgesMapping[inputId] = logicalId++;
                m_inEdges.emplace_back(incomingEdge);
            }
        }

        for(EdgeId id : meta.GetOutEdgesIds())
            m_outEdges.emplace(id, id);
    }

    PyNode::~PyNode() {}

    void PyNode::Invoke() {}

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
