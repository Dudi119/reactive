#include "sweetPy/InitModule.h"
#include "sweetPy/CPythonClass.h"
#include "PyNode.h"

namespace reactive{
    INIT_MODULE(_pyNode, "reactive pyNode")
    {
        sweetPy::CPythonClass <PyNode> pyNode(module, "PyNode", "A py node unit capable of running python logic from with in the graph.");
        pyNode.AddConstructor<PyObject *, const PyFunctionSignature &>();
    }
}
