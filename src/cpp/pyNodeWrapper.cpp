#include "sweetPy/InitModule.h"
#include "sweetPy/CPythonClass.h"
#include "sweetPy/CPythonEnum.h"
#include "PyNode.h"
#include "InputAdapter.h"
#include "ConstNode.h"

namespace reactive{
    INIT_MODULE(_pyNode, "reactive pyNode")
    {
        sweetPy::CPythonClass<PyNodeFactory> pyNodeFactory(module, "PyNodeFactory", "A py node unit factory.");
        pyNodeFactory.AddStaticMethod("create", "Creates PyNode instance", &PyNodeFactory::Create);

        sweetPy::CPythonClass<UnitNode> unitNode(module, "UnitNode", "A basic representation for graph node");
        unitNode.AddMethod("addConsumer", "will wire a consumer", &UnitNode::AddConsumer);
        unitNode.AddMethod("getOutEdge", "will retrieve a abstract representation of a selected out edge", &UnitNode::GetOutEdge);

        sweetPy::CPythonClass<PyFunctionSignature> pySignature(module, "PyFunctionSignature", "Function signature descriptor");
        pySignature.AddMethod("addParam", "Add a new description for a specific param", &PyFunctionSignature::AddParameter);

        sweetPy::CPythonClass<PyNodeEdgesMetaData> pyMeta(module, "PyNodeEdgesMetaData", "A PyFunction unit edges descriptor");
        pyMeta.AddConstructor<std::vector<int>>();

        sweetPy::CPythonEnum parameterUpdate(module, "pySignature_ParameterUpdate");
        parameterUpdate.AddEnumValue("Scalar", PyFunctionSignature::Scalar);
        parameterUpdate.AddEnumValue("Edge", PyFunctionSignature::Edge);

        sweetPy::CPythonClass<ConstNodeFactory> constNodeFactory(module, "ConstNodeFactory", "A ConstNode unit factory.");
        constNodeFactory.AddStaticMethod("create", "Creates ConstNode instance", &ConstNodeFactory::Create);

        sweetPy::CPythonClass<InputAdapter> inputAdapter(module, "InputAdapter", "A basic representation for a graph root");
        inputAdapter.AddConstructor<int>();
        inputAdapter.AddMethod("addConsumer", "will wire a consumer", &InputAdapter::AddConsumer);
        inputAdapter.AddStaticMethod("generateId", "will generate a unique edge id", &InputAdapter::GenerateId);
    }
}
