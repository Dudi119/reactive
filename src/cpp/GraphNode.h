#pragma once

#include <vector>
#include <memory>
#include "Event.h"
#include "GraphEngine.h"

namespace reactive {
    class GraphNode {
    public:
        typedef std::shared_ptr<GraphNode> GraphNode_ptr;

        void AddConsumer(const GraphNode_ptr &node);
        virtual void Consume(const IEvent::Event_ptr &event) = 0;

    private:
        template<typename T>
        void Propegate(const T& data)
        {
            for(const GraphNode_ptr& consumer: m_consumers)
            {
                GraphEngine::Instance().AddEvent(new Event<T>(consumer, data));
            }
        }

    private:
        std::vector<GraphNode_ptr> m_consumers;
    };
}
