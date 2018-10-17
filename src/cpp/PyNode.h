#pragma once

#include <Python.h>
#include <vector>
#include <tuple>
#include <Python.h>
#include "sweetPy/Core/Deleter.h"
#include "UnitNode.h"

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
            Value
        };

        typedef std::vector<std::tuple<ParameterUpdate, PyObject*, PyObject*>> Params;

        ~PyFunctionSignature();
        const Params& GetParams() const;
        void AddParameter(ParameterUpdate parameterUpdate, PyObject* parameterType, PyObject* value);

    private:
        Params m_parametersDescriptors;
    };

    class PyNode : public UnitNode
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
        static UnitNode& Create(PyObject* pyFunction, const PyFunctionSignature& signature);
    };
}