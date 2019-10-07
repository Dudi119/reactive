#include "sweetPy/sweetPy.h"
#include "PyNode.h"
#include "InputAdapter.h"
#include "ConstNode.h"
#include "CurveNode.h"

namespace reactive{

    static PyNode& PyNode_UpCast(UnitNode& node){return static_cast<PyNode&>(node);}

    INIT_MODULE(_pyNode, "reactive pyNode")
    {
        module.add_function("pyNode_UpCast", "will upcast UnitNode& -> PyNode&", &PyNode_UpCast);

        sweetPy::Clazz<PyNodeFactory> pyNodeFactory(module, "PyNodeFactory", "A py node unit factory.");
        pyNodeFactory.add_static_method("create", "Creates PyNode instance", &PyNodeFactory::Create);

        sweetPy::Clazz<PyNode> pyNode(module, "PyNode", "A representation for graph node");
        pyNode.add_method("init", "constructor extension for pyNode", &PyNode::Init);

        sweetPy::Clazz<UnitNode> unitNode(module, "UnitNode", "A basic representation for graph node");
        unitNode.add_method("addConsumer", "will wire a consumer", &UnitNode::AddConsumer);
        unitNode.add_method("getOutEdge", "will retrieve a abstract representation of a selected out edge", &UnitNode::GetOutEdge);
        unitNode.add_static_method("produceOutEdgeData", "Will produce a new event by using a specific out edge", &UnitNode::ProduceOutEdgeData);

        sweetPy::Clazz<PyFunctionSignature> pySignature(module, "PyFunctionSignature", "Function signature descriptor");
        pySignature.add_method("addParam", "Add a new description for a specific param", &PyFunctionSignature::AddParameter);

        sweetPy::Clazz<PyNodeEdgesMetaData> pyMeta(module, "PyNodeEdgesMetaData", "A PyFunction unit edges descriptor");
        pyMeta.add_constructor<std::vector<int>>();

        sweetPy::Enum parameterUpdate(module, "pySignature_ParameterUpdate");
        parameterUpdate.add_value("Scalar", PyFunctionSignature::Scalar);
        parameterUpdate.add_value("Edge", PyFunctionSignature::Edge);

        sweetPy::Clazz<ConstNodeFactory> constNodeFactory(module, "ConstNodeFactory", "A ConstNode unit factory.");
        constNodeFactory.add_static_method("create", "Creates ConstNode instance", &ConstNodeFactory::Create);
    
        sweetPy::Clazz<CurveNodeFactory> curveNodeFactory(module, "CurveNodeFactory", "A CurveNode unit factory.");
        curveNodeFactory.add_static_method("create", "Creates CurveNode instance", &CurveNodeFactory::Create);

        sweetPy::Clazz<InputAdapter> inputAdapter(module, "InputAdapter", "A basic representation for a graph root");
        inputAdapter.add_constructor<int>();
        inputAdapter.add_method("addConsumer", "will wire a consumer", &InputAdapter::AddConsumer);
        inputAdapter.add_static_method("generateId", "will generate a unique edge id", &InputAdapter::GenerateId);
    }
}
