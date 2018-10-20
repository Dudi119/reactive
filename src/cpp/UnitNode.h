#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include "Event.h"
#include "GraphEngine.h"
#include "GraphNode.h"

namespace reactive {

    class InputAdapter;

    class UnitNode : public GraphNode
    {
    public:
        virtual ~UnitNode();
        void AddConsumer(int edgeId, UnitNode& consumer);
        void PreEngineStart() override;
        void PostEngineStart() override;
        void PostEngineStep() override;
        void PostStop() override;
        //virtual void Consume(const IEvent::Event_ptr &event) = 0;

    private:
        /*template<typename T>
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
        }*/

    protected:
        std::unordered_map<int, std::unique_ptr<InputAdapter>> m_inputAdapters;
    };
}
