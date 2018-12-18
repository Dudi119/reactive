#include "CurveNode.h"
#include <chrono>
#include "core/Assert.h"
#include "Event.h"
#include "GraphEngine.h"

namespace reactive{
    CurveNode::CurveNode(int id, const std::vector<sweetPy::object_ptr>& data, const std::vector<sweetPy::TimeDelta>& delta)
            : InputAdapter(id)
    {
        VERIFY(data.size() == delta.size(), "data size doesn't match delta size");
        auto dataIt = data.begin();
        auto deltaIt = delta.begin();
        while(dataIt != data.end())
        {
            m_curve.emplace_back(nullptr, *deltaIt);
            m_curve.back().first = std::move(const_cast<sweetPy::object_ptr&>(*dataIt));
            dataIt++; deltaIt++;
        }
    }
    
    CurveNode::~CurveNode() {}
    
    void CurveNode::PostStart()
    {
        for(auto& curvePair : m_curve)
        {
            std::unique_ptr<Event> event = TypedEventFactory::Create(curvePair.first->ob_type, *this, curvePair.first);
            Event::TimePoint eventTime = std::chrono::system_clock::now() + curvePair.second.GetDuration();
            GraphEngine::Instance().AddTimedEvent(eventTime, event.release());
        }
    }
    
    void CurveNode::PostStop()
    {
        sweetPy::GilLock lock;
        m_curve.clear();
    }
    
    InputAdapter& CurveNodeFactory::Create(int id, const std::vector<sweetPy::object_ptr>& data, const std::vector<sweetPy::TimeDelta>& delta)
    {
        VERIFY(data.size() == delta.size(), "data size doesn't match delta size");
        std::unique_ptr<GraphNode> node(new CurveNode(id, data, delta));
        InputAdapter& nodeRef = static_cast<InputAdapter&>(*node);
        GraphEngine::Instance().RegisterNode(std::move(node));
        return nodeRef;
    }
}

