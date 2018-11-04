#pragma once

#include <list>
#include <vector>
#include <functional>
#include <memory>
#include "GraphNode.h"

namespace reactive{

    class UnitNode;
    class Event;

    class InputAdapter : public GraphNode
    {
    public:
        InputAdapter();
        InputAdapter(int id);
        virtual ~InputAdapter();
        int GetId() const{return m_id;}
        void PreStart() override;
        void PostStart() override;
        void PostStep() override;
        void PostStop() override;
        void AddConsumer(UnitNode& consumer);
        void ConsumeEvent(std::unique_ptr<Event>&& event);
        static int GenerateId();

    private:
        std::list<std::reference_wrapper<UnitNode>> m_consumers;
        std::vector<std::unique_ptr<Event>> m_events;
        int m_id;
    };
}