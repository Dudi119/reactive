#pragma once

#include "Python.h"
#include <vector>
#include <utility>
#include <Python.h>
#include "GraphNode.h"

namespace reactive
{
    struct PyFunctionSignature
    {
    public:
        enum ParameterType
        {
            Scalar,
            Edge
        };

        std::vector<std::pair<ParameterType, PyObject*>> Parameters;
    };

    class PyNode : public GraphNode
    {
        PyNode(PyObject* functionObject, const PyFunctionSignature& signature){}
    };
}