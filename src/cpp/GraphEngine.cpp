#include "GraphEngine.h"
#include <Python.h>
#include "UnitNode.h"

namespace reactive
{
    thread_local std::mutex GraphEngine::m_eventQueueMutex{};
    thread_local std::shared_ptr<GraphEngine::NextCycleQueue> GraphEngine::m_nextCycleEvents{};

    GraphEngine& GraphEngine::Instance()
    {
        static GraphEngine instance;
        return instance;
    }

    GraphEngine::GraphEngine(): m_cycleDuration(2)
    {
        m_eventLoopThread.reset(new core::Thread("GraphEngine Event loop thread", std::bind(&GraphEngine::EventLoop, this)));
        m_eventLoopThread->Start();
    }

    void GraphEngine::InitiateEventQueue()
    {
        m_nextCycleEvents.reset(new NextCycleQueue());
        {
            std::lock_guard<std::mutex> guard(m_eventQueuesMutex);
            m_totalNextCycleEvents.push_back(m_nextCycleEvents);
        }
    }

    void GraphEngine::RegisterNode(std::unique_ptr<GraphNode> &&node)
    {
        m_nodes.emplace_back(node.release());
    }

    void GraphEngine::AddEvent(Event* event)
    {
        if(m_nextCycleEvents.get() == nullptr)
            InitiateEventQueue();
        std::lock_guard<std::mutex> guard(m_eventQueueMutex);
        m_nextCycleEvents->push_back(event);
    }

    void GraphEngine::AddTimedEvent(const Event::TimePoint &timePoint, Event *event)
    {
        m_timedEvents.insert(std::make_pair(timePoint, event));
    }

    void GraphEngine::Stop()
    {
        Py_BEGIN_ALLOW_THREADS
        {
            std::unique_lock<std::mutex> guard(m_eventLoopMutex);
            m_isStopped = true;
            m_eventLoopConv.notify_one();
        }
        m_eventLoopThread->Join();
        Py_END_ALLOW_THREADS
    }

    void GraphEngine::EventLoop()
    {
        typedef std::chrono::milliseconds Milliseconds;
        {
            std::unique_lock<std::mutex> guard(m_eventLoopMutex);
            while(m_isStopped == false)
            {
                auto cycleStartingTime = std::chrono::system_clock::now();
                Milliseconds currentCycleDuration = std::chrono::duration_cast<Milliseconds>(std::chrono::system_clock::now() - cycleStartingTime);
                Milliseconds waitingTime = currentCycleDuration > m_cycleDuration ? m_cycleDuration : m_cycleDuration - currentCycleDuration;
                m_eventLoopConv.wait_until(guard, std::chrono::system_clock::now() + waitingTime, [this]{return m_isStopped;});
            }
        }
        PostStop();
    }

    void GraphEngine::PostStop()
    {
        for(auto& node : m_nodes)
           node->PostStop();
    }
}
