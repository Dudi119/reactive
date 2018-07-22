#pragma once

#include <memory>

namespace reactive
{
    class GraphNode;

    class IEvent
    {
    public:
        typedef std::shared_ptr<IEvent> Event_ptr;
        virtual ~IEvent(){}
    };

    template<typename T>
    class Event : public IEvent
    {
    public:
        Event(const std::shared_ptr<GraphNode> &consumer, T &&data)
                :m_consumer(consumer), m_data(std::move(data)) {}
        virtual ~Event(){}
        std::shared_ptr<GraphNode>& GetConsumer() const{ return m_consumer; }
        T& GetData() const { return m_data; }

    private:
        std::shared_ptr<GraphNode> m_consumer;
        T m_data;
    };
}