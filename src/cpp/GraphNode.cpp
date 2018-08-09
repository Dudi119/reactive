#include "GraphNode.h"
#include "GraphEngine.h"

namespace reactive
{
    void GraphNode::AddConsumer(const GraphNode_ptr &node)
    {
        m_consumers.push_back(node);
    }
}
