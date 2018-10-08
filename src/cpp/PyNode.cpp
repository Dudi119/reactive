#include "PyNode.h"
#include <memory>
#include "GraphEngine.h"

namespace reactive{
    PyFunctionSignature::~PyFunctionSignature()
    {
        for(auto& paramDescriptor : m_parametersDescriptors)
        {
            Py_DECREF(std::get<Type>(paramDescriptor));
            Py_DECREF(std::get<ClosureValue>(paramDescriptor));
        }
    }
    void PyFunctionSignature::AddParameter(ParameterUpdate parameterUpdate, PyObject* parameterType, PyObject *closureValue)
    {
        Py_XINCREF(parameterType);
        Py_XINCREF(closureValue);
        m_parametersDescriptors.emplace_back(std::make_tuple(parameterUpdate, parameterType, closureValue));
    }

    PyNode::PyNode(sweetPy::object_ptr &&pyFunction, const PyFunctionSignature &signature)
        :m_pyFunction(std::move(pyFunction))
    {
    }

    PyNode::~PyNode() {}

    void PyNode::Stop()
    {
        m_pyFunction.reset();
    }

    GraphNode& PyNodeFactory::Create(PyObject *pyFunction, const PyFunctionSignature &signature)
    {
        Py_XINCREF(pyFunction);
        std::unique_ptr<GraphNode> node(new PyNode(sweetPy::object_ptr(pyFunction, &sweetPy::Deleter::Owner), signature));
        GraphNode& nodeRef = *node;
        GraphEngine::Instance().RegisterNode(std::move(node));
        return nodeRef;
    }
}
