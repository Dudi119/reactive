#pragma once

#include <Python.h>
#include <vector>
#include <tuple>
#include <Python.h>
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
        PyNode(PyObject* functionObject, const PyFunctionSignature& signature){}
    };
}