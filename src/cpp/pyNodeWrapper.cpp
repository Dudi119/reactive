#include "sweetPy/InitModule.h"
#include "sweetPy/CPythonClass.h"
#include "sweetPy/CPythonEnum.h"
#include "sweetPy/CPythonGlobalFunction.h"
#include "PyNode.h"
#include "InputAdapter.h"
#include "ConstNode.h"

namespace reactive{

    static PyNode& PyNode_UpCast(UnitNode& node){return static_cast<PyNode&>(node);}

    INIT_MODULE(_pyNode, "reactive pyNode")
    {
        sweetPy::CPythonGlobalFunction(module, "pyNode_UpCast", "will upcast UnitNode& -> PyNode&", &PyNode_UpCast);

        sweetPy::CPythonClass<PyNodeFactory> pyNodeFactory(module, "PyNodeFactory", "A py node unit factory.");
        pyNodeFactory.AddStaticMethod("create", "Creates PyNode instance", &PyNodeFactory::Create);

        sweetPy::CPythonClass<PyNode> pyNode(module, "PyNode", "A representation for graph node");
        pyNode.AddMethod("init", "constructor extension for pyNode", &PyNode::Init);

        sweetPy::CPythonClass<UnitNode> unitNode(module, "UnitNode", "A basic representation for graph node");
        unitNode.AddMethod("addConsumer", "will wire a consumer", &UnitNode::AddConsumer);
        unitNode.AddMethod("getOutEdge", "will retrieve a abstract representation of a selected out edge", &UnitNode::GetOutEdge);
        unitNode.AddStaticMethod("produceOutEdgeData", "Will produce a new event by using a specific out edge", &UnitNode::ProduceOutEdgeData);

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
