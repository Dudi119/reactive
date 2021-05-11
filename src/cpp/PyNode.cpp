#include "PyNode.h"
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

    PyNode::~PyNode() {}

    void PyNode::Init(const std::string& serializedFunction, const reactive::PyFunctionSignature &signature, const reactive::PyNodeEdgesMetaData &meta)
    {
        m_serializedFunction.reset(new std::string(serializedFunction));
        m_concurrent = true;
        _Init(signature, meta);
    }

    void PyNode::Init(PyObject* pyFunction, const reactive::PyFunctionSignature &signature, const reactive::PyNodeEdgesMetaData &meta)
    {
        Py_XINCREF(pyFunction);
        m_pyFunction = sweetPy::ObjectPtr(pyFunction, &sweetPy::Deleter::Owner);
        _Init(signature, meta);
    }

    void PyNode::_Init(const reactive::PyFunctionSignature &signature, const reactive::PyNodeEdgesMetaData &meta)
    {
        static LogicalId logicalId = 0;
        auto& params = signature.GetParams();
        for(int paramId = 0; paramId <  params.size(); paramId++)
        {
            if(std::get<PyFunctionSignature::Update>(params[paramId]) == PyFunctionSignature::Edge)
            {
                InputAdapter& incomingEdge = sweetPy::Object<InputAdapter&>::from_python(std::get<PyFunctionSignature::Value>(params[paramId]));
                EdgeId inputId = incomingEdge.GetId();
                if(m_inEdgesMapping.find(inputId) != m_inEdgesMapping.end())
                throw core::Exception(__CORE_SOURCE, "Input edge already exists - %d", inputId);
                m_inEdgesMapping[inputId] = logicalId++;
                m_inEdges.emplace_back(incomingEdge);
                m_arguments.add_element(reinterpret_cast<void*>(&incomingEdge), [](void const * const ptr)->PyObject*{
                    const InputAdapter& inputAdapter = *reinterpret_cast<InputAdapter const *>(ptr);
                    return inputAdapter.GetLastData().release();
                });
            }
            else //Scalar
            {
                PyObject* scalar = std::get<PyFunctionSignature::Value>(params[paramId]);
                m_arguments.add_element(sweetPy::ObjectPtr(scalar, &sweetPy::Deleter::Borrow));
            }
        }

        for(EdgeId id : meta.GetOutEdgesIds())
            m_outEdges.emplace(id, id);
    }

    void PyNode::Invoke()
    {
        if(m_concurrent)
        {
            GraphEngine::Instance().InvokeConcurrentFunction(*m_serializedFunction);
        }
        else
        {
            sweetPy::GilLock lock;
            sweetPy::Python::fast_invoke_function(m_pyFunction, m_arguments);
        }
    }

    void PyNode::PostStop()
    {
        sweetPy::GilLock lock;
        m_pyFunction.reset();
    }

    void PyNode::PostInvoke()
    {
        UnitNode::PostInvoke();
        for(auto& outEdgePair : m_outEdges)
            outEdgePair.second.PostInvoke();
    }

    UnitNode& PyNodeFactory::Create()
    {
        std::unique_ptr<GraphNode> node(new PyNode());
        UnitNode& nodeRef = static_cast<UnitNode&>(*node);
        GraphEngine::Instance().RegisterNode(std::move(node));
        return nodeRef;
    }
}