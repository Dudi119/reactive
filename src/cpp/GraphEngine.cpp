#include "GraphEngine.h"
#include <thread>
#include "sweetPy/Core/Lock.h"
#include "UnitNode.h"
#include "InputAdapter.h"

namespace reactive
{
    thread_local std::mutex GraphEngine::m_eventQueueMutex{};
    thread_local std::shared_ptr<GraphEngine::NextCycleQueue> GraphEngine::m_nextCycleEvents{};

    GraphEngine& GraphEngine::Instance()
    {
        static GraphEngine instance;
        return instance;
    }

    GraphEngine::~GraphEngine()
    {
        Stop();
    }

    GraphEngine::GraphEngine(): m_cycleDuration(2), m_isStarted(false)
    {
        m_eventLoopThread.reset(new core::Thread("GraphEngine Event loop thread", std::bind(&GraphEngine::EventLoop, this)));
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

    void GraphEngine::AddNodeToCycle(GraphNode &node)
    {
        if(node.IsTicked() == false) //Making sure we only going to add each node once.
        {
            m_currentCycleNodes.emplace_front(node);
            node.SetTicked();
        }
    }

    void GraphEngine::AddTimedEvent(const Event::TimePoint &timePoint, Event *event)
    {
        m_timedEvents.insert(std::make_pair(timePoint, event));
    }

    void GraphEngine::Stop()
    {
        if(m_isStopped == true || m_isStarted == false)
            return;
        sweetPy::Yield yield;
        {
            std::unique_lock<std::mutex> guard(m_eventLoopMutex);
            if(m_isStopped == true)
                return;
            m_isStopped = true;
            m_eventLoopConv.notify_one();
        }
        m_eventLoopThread->Join();
    }

    void GraphEngine::ConsumeTimedEvents(const Event::TimePoint& upperTimePoint)
    {
        while( m_timedEvents.empty() == false && m_timedEvents.begin()->first < upperTimePoint )
        {
            auto it = m_timedEvents.begin();
            InputAdapter& inputAdapter = static_cast<InputAdapter&>(it->second->GetConsumer());
            inputAdapter.ConsumeEvent(std::unique_ptr<Event>(it->second));
            m_timedEvents.erase(it);
        }
    }

    void GraphEngine::Start(const sweetPy::DateTime& endTime)
    {
        sweetPy::Yield yield;
        m_eventLoopThread->Start();
        m_isStarted = true;
        std::this_thread::sleep_for(endTime.GetDuration());
        Stop();
    }

    void GraphEngine::EventLoop()
    {
        PostStart();
        typedef std::chrono::milliseconds Milliseconds;
        {
            std::unique_lock<std::mutex> guard(m_eventLoopMutex);
            while(m_isStopped == false)
            {
                auto cycleStartingTime = std::chrono::system_clock::now();
                ConsumeTimedEvents(cycleStartingTime + m_cycleDuration);
                Milliseconds currentCycleDuration = std::chrono::duration_cast<Milliseconds>(std::chrono::system_clock::now() - cycleStartingTime);
                Milliseconds waitingTime = currentCycleDuration > m_cycleDuration ? Milliseconds(0) : m_cycleDuration - currentCycleDuration;
                m_eventLoopConv.wait_until(guard, std::chrono::system_clock::now() + waitingTime, [this]{return m_isStopped;});
            }
        }
        PostStop();
    }

    void GraphEngine::PostStart()
    {
        for(auto& node : m_nodes)
            node->PostStart();
    }

    void GraphEngine::PostStop()
    {
        for(auto& node : m_nodes)
           node->PostStop();
    }
}
