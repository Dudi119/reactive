#include "ConstNode.h"
#include <chrono>
#include "Event.h"
#include "GraphEngine.h"

namespace reactive{
    ConstNode::ConstNode(sweetPy::object_ptr &&value, const sweetPy::TimeDelta &delta)
        : m_value(std::move(value)), m_delta(delta)
    {}

    ConstNode::~ConstNode() {}

    void ConstNode::PostStart()
    {
        std::unique_ptr<Event> event = TypedEventFactory::Create(m_value->ob_type, *this, m_value);
        Event::TimePoint eventTime = std::chrono::system_clock::now() + m_delta.GetDuration();
        GraphEngine::Instance().AddTimedEvent(eventTime, event.release());
    }
}
