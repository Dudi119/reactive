#include "UnitNode.h"
#include <Python.h>
#include "core/Exception.h"
#include "Event.h"

namespace reactive
{
    UnitNode::~UnitNode() {}

    void UnitNode::AddConsumer(EdgeId id, UnitNode& consumer)
    {
        if(m_outEdges.find(id) == m_outEdges.end())
            throw core::Exception(__CORE_SOURCE, "None existing output edge is requested - %d", id);
        m_outEdges[id].AddConsumer(consumer);
    }

    void UnitNode::MarkInputEdge(EdgeId id)
    {
        if(m_inEdgesMapping.find(id) == m_inEdgesMapping.end())
            throw core::Exception(__CORE_SOURCE, "None existing input edge is requested - %d", id);
        m_tickedInput[m_inEdgesMapping[id]] = true;
    }

    InputAdapter& UnitNode::GetOutEdge(EdgeId id)
    {
       if(m_outEdges.find(id) == m_outEdges.end())
           throw core::Exception(__CORE_SOURCE, "None existing output edge is requested - %d", id);
        return m_outEdges[id];
    }

    void UnitNode::ProduceOutEdgeData(UnitNode& node, int outEdgeId, PyObject* data)
    {
        InputAdapter& out = node.GetOutEdge(outEdgeId);
        std::unique_ptr<Event> event = TypedEventFactory::Create(Py_TYPE(data), out, sweetPy::ObjectPtr(data, &sweetPy::Deleter::Borrow));
        out.ConsumeEvent(std::move(event));
    }

    void UnitNode::PreStart(){}
    void UnitNode::PostStart(){}
    void UnitNode::PostInvoke(){ m_tickedInput.reset(); GraphNode::PostInvoke(); }
    void UnitNode::PostStep(){ m_tickedInput.reset(); GraphNode::PostStep(); }
    void UnitNode::PostStop() {}
    void UnitNode::Invoke() {throw core::Exception(__CORE_SOURCE, "Not implemented");}
}
