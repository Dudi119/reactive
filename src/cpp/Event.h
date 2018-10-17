#pragma once

#include <memory>

namespace reactive
{
    class UnitNode;

    class Event
    {
    public:
        typedef std::shared_ptr<Event> Event_ptr;
        Event(UnitNode& consumer);
        virtual ~Event();
        const UnitNode& GetConsumer() const{ return m_consumer; }

    private:
        UnitNode& m_consumer;
    };

    template<typename T>
    class TypedEvent : public Event
    {
    public:
        TypedEvent(UnitNode& consumer, T &&data);
        virtual ~TypedEvent();
        T& GetData() const { return m_data; }

    private:
        T m_data;
    };
}