#include "GraphEngine.h"

namespace reactive
{
    thread_local std::mutex GraphEngine::m_eventQueueMutex{};
    thread_local std::shared_ptr<GraphEngine::NextCycleQueue> GraphEngine::m_nextCycleEvents{};

    GraphEngine& GraphEngine::Instance()
    {
        static GraphEngine instance;
        return instance;
    }

    GraphEngine::GraphEngine(): m_cycleDuration(2) {}

    void GraphEngine::InitiateEventQueue()
    {
        m_nextCycleEvents.reset(new NextCycleQueue());
        {
            std::lock_guard<std::mutex> guard(m_totalNextCycleEventsMutex);
            m_totalNextCycleEvents.push_back(m_nextCycleEvents);
        }
    }

    void GraphEngine::AddEvent(const IEvent::Event_ptr &event)
    {
        m_nextCycleEvents->push_back(event);
    }
}
