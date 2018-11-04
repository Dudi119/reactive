#include "InputAdapter.h"
#include <atomic>
#include "Event.h"
#include "UnitNode.h"
#include "GraphEngine.h"

namespace reactive{

    InputAdapter::InputAdapter():m_id(-1){}
    InputAdapter::InputAdapter(int id):m_id(id){}
    InputAdapter::~InputAdapter() {}

    void InputAdapter::PreStart(){}
    void InputAdapter::PostStart(){}
    void InputAdapter::PostStep()
    {
        GraphNode::PostStep();
    }
    void InputAdapter::PostStop() {}

    void InputAdapter::AddConsumer(UnitNode &consumer)
    {
        m_consumers.emplace_back(consumer);
    }

    void InputAdapter::ConsumeEvent(std::unique_ptr<Event> &&event)
    {
        m_events.emplace_back(std::move(event));
        for( auto& consumer : m_consumers )
        {
            consumer.get().MarkInputEdge(m_id);
            GraphEngine::Instance().AddNodeToCycle(static_cast<GraphNode&>(consumer));
        }
    }

    int InputAdapter::GenerateId()
    {
        static std::atomic_int id(0);
        return id++;
    }
}