#include "GraphNode.h"

namespace reactive
{

    void GraphNode::AddConsumer(const GraphNode_ptr &node)
    {
        m_consumers.push_back(node);
    }
}
