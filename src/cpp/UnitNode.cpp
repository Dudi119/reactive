#include "UnitNode.h"
#include "core/Exception.h"

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

    void UnitNode::PreStart(){}
    void UnitNode::PostStart(){}
    void UnitNode::PostStep()
    {
        GraphNode::PostStep();
    }
    void UnitNode::PostStop() {}
}
