#pragma once

#include <list>
#include <vector>
#include <functional>
#include <memory>
#include "sweetPy/sweetPy.h"
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
        InputAdapter(const InputAdapter& obj);
        virtual ~InputAdapter();
        int GetId() const{return m_id;}
        void PreStart() override;
        void PostStart() override;
        void PostStop() override;
        void Invoke() override;
        void AddConsumer(UnitNode& consumer);
        bool ConsumeEvent(std::unique_ptr<Event>&& event);
        std::vector<std::shared_ptr<Event>> GetEvents();
        sweetPy::ObjectPtr GetLastData() const;
        static int GenerateId();

    private:
        std::list<std::reference_wrapper<UnitNode>> m_consumers;
        std::unique_ptr<Event> m_event;
        int m_id;
        ConsumeType m_consumeType;
    };
}