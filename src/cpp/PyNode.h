#pragma once

#include <Python.h>
#include <vector>
#include <tuple>
#include "sweetPy/sweetPy.h"
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
        virtual ~PyNode();

        void Init(const std::string& serializedFunction, const PyFunctionSignature& signature, const PyNodeEdgesMetaData& meta);
        void Init(PyObject* pyFunction, const PyFunctionSignature& signature, const PyNodeEdgesMetaData& meta);
        void PostStop() override;
        void PostInvoke() override;
        void Invoke() override;

    private:
        void _Init(const PyFunctionSignature& signature, const PyNodeEdgesMetaData& meta);

    private:
        sweetPy::ObjectPtr m_pyFunction;
        sweetPy::Tuple m_arguments;
        std::unique_ptr<std::string> m_serializedFunction;
        bool m_concurrent;
    };

    class PyNodeFactory
    {
    public:
        static UnitNode& Create();
    };
}