#pragma once

#include <Python.h>
#include <vector>
#include <tuple>
#include <Python.h>
#include "sweetPy/Core/Deleter.h"
#include "GraphNode.h"

namespace reactive
{
    class PyFunctionSignature
    {
    public:
        enum ParameterUpdate
        {
            Scalar,
            Edge
        };

        enum DescriptorOffset
        {
            Update,
            Type,
            ClosureValue
        };
        ~PyFunctionSignature();
        void AddParameter(ParameterUpdate parameterUpdate, PyObject* parameterType, PyObject* closureValue);

    private:
        std::vector<std::tuple<ParameterUpdate, PyObject*, PyObject*>> m_parametersDescriptors;
    };

    class PyNode : public GraphNode
    {
    public:
        PyNode(sweetPy::object_ptr&& pyFunction, const PyFunctionSignature& signature);
        virtual ~PyNode();

        void Stop() override;

    private:
        sweetPy::object_ptr m_pyFunction;
    };

    class PyNodeFactory
    {
    public:
        static GraphNode& Create(PyObject* pyFunction, const PyFunctionSignature& signature);
    };
}