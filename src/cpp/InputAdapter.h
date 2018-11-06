#pragma once

#include <list>
#include <vector>
#include <functional>
#include <memory>
#include "GraphNode.h"

namespace reactive{

    class UnitNode;
    class Event;

    enum class ConsumeType: short
    {
        Non_Collapse,
        Collapse
    };

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
        void Invoke() override;
        void AddConsumer(UnitNode& consumer);
        bool ConsumeEvent(std::unique_ptr<Event>&& event);
        std::vector<std::shared_ptr<Event>> GetEvents();
        static int GenerateId();

    private:
        std::list<std::reference_wrapper<UnitNode>> m_consumers;
        std::vector<std::shared_ptr<Event>> m_events;
        int m_id;
        bool m_isTicked;
        ConsumeType m_consumeType;
    };
}