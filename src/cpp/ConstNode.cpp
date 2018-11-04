#include "ConstNode.h"
#include <chrono>
#include "Event.h"
#include "GraphEngine.h"

namespace reactive{
    ConstNode::ConstNode(int id, sweetPy::object_ptr &&value, const sweetPy::TimeDelta &delta)
        : InputAdapter(id), m_value(std::move(value)), m_delta(delta)
    {}

    ConstNode::~ConstNode() {}

    void ConstNode::PostStart()
    {
        std::unique_ptr<Event> event = TypedEventFactory::Create(m_value->ob_type, *this, m_value);
        Event::TimePoint eventTime = std::chrono::system_clock::now() + m_delta.GetDuration();
        GraphEngine::Instance().AddTimedEvent(eventTime, event.release());
    }

    void ConstNode::PostStop()
    {
        sweetPy::GilLock lock;
        m_value.reset();
    }

    InputAdapter& ConstNodeFactory::Create(int id, PyObject *value, const sweetPy::TimeDelta &delta)
    {
        Py_XINCREF(value);
        std::unique_ptr<GraphNode> node(new ConstNode(id, sweetPy::object_ptr(value, &sweetPy::Deleter::Owner), delta));
        InputAdapter& nodeRef = static_cast<InputAdapter&>(*node);
        GraphEngine::Instance().RegisterNode(std::move(node));
        return nodeRef;
    }
}
