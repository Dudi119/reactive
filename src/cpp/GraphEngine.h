#pragma once

#include <vector>
#include <mutex>
#include <chrono>
#include "Event.h"

namespace reactive
{
    class GraphEngine
    {
    public:
        typedef std::vector<IEvent::Event_ptr> NextCycleQueue;
        static GraphEngine& Instance();
        void AddEvent(const IEvent::Event_ptr& event);
        void InitiateEventQueue();

    private:
        GraphEngine();

    private:
        thread_local static std::mutex m_eventQueueMutex;
        thread_local static std::shared_ptr<NextCycleQueue> m_nextCycleEvents;
        std::mutex m_totalNextCycleEventsMutex;
        std::vector<std::weak_ptr<NextCycleQueue>> m_totalNextCycleEvents;
        const std::chrono::seconds m_cycleDuration;
    };
}
