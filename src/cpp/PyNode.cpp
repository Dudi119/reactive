#include "PyNode.h"
#include <memory>
#include "sweetPy/CPythonObject.h"
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

    PyNode::PyNode(sweetPy::object_ptr &&pyFunction, const PyFunctionSignature &signature)
        :m_pyFunction(std::move(pyFunction))
    {
        for(auto& paramDescriptor : signature.GetParams())
        {
            if(std::get<PyFunctionSignature::Update>(paramDescriptor) == PyFunctionSignature::Edge)
            {
                int inputId = sweetPy::Object<int>::FromPython(std::get<PyFunctionSignature::Value>(paramDescriptor));
                if(m_inputAdapters.find(inputId) == m_inputAdapters.end())
                    m_inputAdapters.emplace(inputId, std::unique_ptr<InputAdapter>(new InputAdapter()));
            }
        }
    }

    PyNode::~PyNode() {}

    void PyNode::Stop()
    {
        m_pyFunction.reset();
    }

    UnitNode& PyNodeFactory::Create(PyObject *pyFunction, const PyFunctionSignature &signature)
    {
        Py_XINCREF(pyFunction);
        std::unique_ptr<GraphNode> node(new PyNode(sweetPy::object_ptr(pyFunction, &sweetPy::Deleter::Owner), signature));
        UnitNode& nodeRef = static_cast<UnitNode&>(*node);
        GraphEngine::Instance().RegisterNode(std::move(node));
        return nodeRef;
    }
}
