#include "PyNode.h"

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
}
