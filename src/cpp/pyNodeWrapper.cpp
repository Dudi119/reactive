#include "sweetPy/InitModule.h"
#include "sweetPy/CPythonClass.h"
#include "sweetPy/CPythonEnum.h"
#include "PyNode.h"

namespace reactive{
    INIT_MODULE(_pyNode, "reactive pyNode")
    {
        sweetPy::CPythonClass<PyNode> pyNode(module, "PyNode", "A py node unit capable of running python logic from with in the graph.");
        pyNode.AddConstructor<PyObject *, const PyFunctionSignature &>();

        sweetPy::CPythonClass<PyFunctionSignature> pySignature(module, "PyFunctionSignature", "Function signature descriptor");
        pySignature.AddMethod("addParam", "Add a new description for a specific param", &PyFunctionSignature::AddParameter);

        sweetPy::CPythonEnum parameterUpdate(module, "pySignature_ParameterUpdate");
        parameterUpdate.AddEnumValue("Scalar", PyFunctionSignature::Scalar);
        parameterUpdate.AddEnumValue("Edge", PyFunctionSignature::Edge);
    }
}
