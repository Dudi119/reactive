#pragma once

#include <memory>
#include <chrono>

namespace reactive
{
    class UnitNode;

    class Event
    {
    public:
        typedef std::chrono::system_clock::time_point TimePoint;

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