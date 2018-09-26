#include "GraphNode.h"

namespace reactive
{
    void GraphNode::AddConsumer(int edgeId, const GraphNode_ptr &node)
    {
        m_consumers[edgeId].push_back(node);
    }
}
