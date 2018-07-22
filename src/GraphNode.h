#pragma once

#include <vector>
#include <memory>
#include "Event.h"

namespace reactive {
    class GraphNode {
    public:
        typedef std::shared_ptr<GraphNode> GraphNode_ptr;

        void AddConsumer(const GraphNode_ptr &node);
        template<typename T>
        void Consume(const T &data)
        {

        }
        void Propegate(){}

    private:
        std::vector<GraphNode_ptr> m_consumers;
    };
}
