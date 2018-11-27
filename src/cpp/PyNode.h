#pragma once

#include <Python.h>
#include <vector>
#include <tuple>
#include "sweetPy/Core/Deleter.h"
#include "sweetPy/Types/Tuple.h"
#include "UnitNode.h"

namespace reactive
{
    class PyNodeEdgesMetaData
    {
    public:
        PyNodeEdgesMetaData(std::vector<int> outEdgesIds):m_outEdgesIds(outEdgesIds){}
        const std::vector<int> GetOutEdgesIds() const {return m_outEdgesIds;}

    private:
        std::vector<int> m_outEdgesIds;
    };

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
        PyNode(sweetPy::object_ptr&& pyFunction, const PyFunctionSignature& signature, const PyNodeEdgesMetaData& meta);
        virtual ~PyNode();

        void PostStop() override;
        void Invoke() override;

    private:
        sweetPy::object_ptr m_pyFunction;
        sweetPy::Tuple m_arguments;
    };

    class PyNodeFactory
    {
    public:
        static UnitNode& Create(PyObject* pyFunction, const PyFunctionSignature& signature, const PyNodeEdgesMetaData& meta);
    };
}