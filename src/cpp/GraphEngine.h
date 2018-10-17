#pragma once

#include <vector>
#include <mutex>
#include <chrono>
#include "Event.h"

namespace reactive
{
    class GraphNode;

    class GraphEngine
    {
    public:
        typedef std::vector<Event::Event_ptr> NextCycleQueue;

        static GraphEngine& Instance();
        void RegisterNode(std::unique_ptr<GraphNode>&& node);
        void AddEvent(const Event::Event_ptr& event);
        void InitiateEventQueue();
        void Stop();

    private:
        GraphEngine();

    private:
        thread_local static std::mutex m_eventQueueMutex;
        thread_local static std::shared_ptr<NextCycleQueue> m_nextCycleEvents;
        std::mutex m_totalNextCycleEventsMutex;
        std::vector<std::weak_ptr<NextCycleQueue>> m_totalNextCycleEvents;
        std::vector<std::unique_ptr<GraphNode>> m_nodes;
        const std::chrono::seconds m_cycleDuration;
    };
}
