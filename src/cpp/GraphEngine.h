#pragma once

#include <vector>
#include <map>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <functional>
#include "core/Thread.h"
#include "sweetPy/Types/TimeDelta.h"
#include "Event.h"

namespace reactive
{
    class GraphNode;

    class GraphEngine
    {
    public:
        typedef std::vector<Event*> NextCycleQueue;
        typedef std::list<std::reference_wrapper<GraphNode>>  CurrentCycleNodes;
        typedef std::list<std::reference_wrapper<GraphNode>>  CurrentCycleRoots;
        typedef std::vector<std::reference_wrapper<GraphNode>> Consumers;

        static GraphEngine& Instance();
        ~GraphEngine();
        void RegisterNode(std::unique_ptr<GraphNode>&& node);
        void AddTimedEvent(const Event::TimePoint& timePoint, Event* event);
        void AddEvent(Event* event);
        void AddNodeToCycle(GraphNode& node);
        void AddRootToCycle(GraphNode& root);
        void Stop();
        void Start(const sweetPy::TimeDelta& endTime);

    private:
        void InitiateEventQueue();
        GraphEngine();
        void EventLoop();
        void PostStart();
        void PostStop();
        void ConsumeTimedEvents(const Event::TimePoint& upperTimePoint);
        void InvokeCurrentCycle();
        void PostCurrentCycle();

    private:
        thread_local static std::mutex m_eventQueueMutex;
        thread_local static std::shared_ptr<NextCycleQueue> m_nextCycleEvents;
        std::multimap<Event::TimePoint, Event*> m_timedEvents;
        std::mutex m_eventQueuesMutex;
        std::vector<std::weak_ptr<NextCycleQueue>> m_totalNextCycleEvents;
        std::vector<std::unique_ptr<GraphNode>> m_nodes;
        CurrentCycleNodes m_currentCycleNodes;
        CurrentCycleRoots m_currentCycleRoots;
        const std::chrono::milliseconds m_cycleDuration;
        bool m_isStarted;
        bool m_isStopped;
        std::mutex m_eventLoopMutex;
        std::condition_variable m_eventLoopConv;
        std::unique_ptr<core::Thread> m_eventLoopThread;
    };
}
