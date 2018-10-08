#include "GraphNode.h"

namespace reactive
{
    GraphNode::~GraphNode() {}

    void GraphNode::AddConsumer(int edgeId, const GraphNode_ptr &node)
    {
        m_consumers[edgeId].push_back(node);
    }
}
