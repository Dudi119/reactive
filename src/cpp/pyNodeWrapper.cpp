#include "sweetPy/InitModule.h"
#include "sweetPy/CPythonClass.h"
#include "sweetPy/CPythonEnum.h"
#include "PyNode.h"

namespace reactive{
    INIT_MODULE(_pyNode, "reactive pyNode")
    {
        sweetPy::CPythonClass<PyNodeFactory> pyNodeFactory(module, "PyNodeFactory", "A py node unit factory.");
        pyNodeFactory.AddStaticMethod("create", "Creates PyNode instance", &PyNodeFactory::Create);

        sweetPy::CPythonClass<PyFunctionSignature> pySignature(module, "PyFunctionSignature", "Function signature descriptor");
        pySignature.AddMethod("addParam", "Add a new description for a specific param", &PyFunctionSignature::AddParameter);

        sweetPy::CPythonEnum parameterUpdate(module, "pySignature_ParameterUpdate");
        parameterUpdate.AddEnumValue("Scalar", PyFunctionSignature::Scalar);
        parameterUpdate.AddEnumValue("Edge", PyFunctionSignature::Edge);
    }
}
