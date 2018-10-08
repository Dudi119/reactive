#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include "core/Exception.h"
#include "Event.h"
#include "GraphEngine.h"

namespace reactive {
    class GraphNode
    {
    public:
        typedef std::shared_ptr<GraphNode> GraphNode_ptr;

        virtual ~GraphNode();
        virtual void Stop() = 0;
        void AddConsumer(int edgeId, const GraphNode_ptr &node);
        //virtual void Consume(const IEvent::Event_ptr &event) = 0;

    private:
        template<typename T>
        void Propegate(int edgeId, const T& data)
        {
            if(m_consumers.find(edgeId) != m_consumers.end())
            {
                for(const GraphNode_ptr& consumer: m_consumers[edgeId])
                {
                    GraphEngine::Instance().AddEvent(new Event<T>(consumer, data));
                }
            }
            else
                throw core::Exception(__CORE_SOURCE, "No edge which supports edgeId - %d exists", edgeId);
        }

    private:
        std::unordered_map<int, std::vector<GraphNode_ptr>> m_consumers;
    };
}
