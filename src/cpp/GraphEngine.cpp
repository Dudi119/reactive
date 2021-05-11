#include "GraphEngine.h"
#include <thread>
#include "UnitNode.h"
#include "InputAdapter.h"

namespace reactive
{
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

    GraphEngine::GraphEngine(): m_cycleDuration(2), m_isStarted(false), m_tasksOrchestrator(true)
    {
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
    
    void GraphEngine::AddRootToCycle(GraphNode &node)
    {
        m_currentCycleRoots.emplace_front(node);
    }

    void GraphEngine::AddTimedEvent(const Event::TimePoint &timePoint, Event *event)
    {
        m_timedEvents.insert(std::make_pair(timePoint, event));
    }

    void GraphEngine::Dispose()
    {
        m_nodes.clear();
    }

    void GraphEngine::Stop()
    {
        if(m_isStopped == true || m_isStarted == false)
            return;
        sweetPy::GilRelease release;
        {
            std::unique_lock<std::mutex> guard(m_eventLoopMutex);
            if(m_isStopped == true)
                return;
            m_isStopped = true;
            m_eventLoopConv.notify_one();
        }
        m_eventLoopThread->join();
    }

    void GraphEngine::ConsumeTimedEvents(const Event::TimePoint& upperTimePoint)
    {
        if(m_timedEvents.empty() == false)
        {
            auto it = m_timedEvents.begin();
            while(it != m_timedEvents.end() && it->first < upperTimePoint )
            {
                InputAdapter& inputAdapter = static_cast<InputAdapter&>(it->second->GetConsumer());
                std::unique_ptr<Event> event(it->second);
                bool consumed = inputAdapter.ConsumeEvent(std::move(event));
                event.release(); //the unique_ptr shouldn't have owenership, it may remain under m_timedEvents
                it = consumed ? m_timedEvents.erase(it) : ++it;
                if(consumed)
                    GraphEngine::Instance().AddRootToCycle(inputAdapter);
            }
        }
    }

    void GraphEngine::InvokeCurrentCycle()
    {
        CurrentCycleNodes::iterator it;
        while(m_currentCycleNodes.empty() == false)
        {
            it = m_currentCycleNodes.begin();
            if(it->get().IsExecuted())
            {
                it->get().PostInvoke();
                m_currentCycleNodes.erase(it);
            }
            else
            {
                it->get().Invoke();
                it->get().SetExecuted();
            }
        }
    }
    
    void GraphEngine::PostCurrentCycle()
    {
        for(auto& root : m_currentCycleRoots)
        {
            root.get().PostInvoke();
        }
        m_currentCycleRoots.clear();
    }

    void GraphEngine::Start(const sweetPy::TimeDelta& endTime, bool forceStart)
    {
        sweetPy::GilRelease release;
        if(m_isStarted && !forceStart)
            throw core::Exception(__CORE_SOURCE, "Engine already initiated");
        
        m_eventLoopThread.reset(new core::Thread("GraphEngine Event loop thread", std::bind(&GraphEngine::EventLoop, this)));
        m_isStarted = true;
        m_isStopped = false;
        std::this_thread::sleep_for(endTime.get_duration());
        Stop();
    }

    void GraphEngine::InvokeConcurrentFunction(const std::string& function)
    {
        ConcurrentOrchestrator::PointerType concurrentTask(MakeConcurrentFunction<ConcurrentOrchestrator::TASK_SIZE>(function));
        m_tasksOrchestrator.Dispatch(std::move(*concurrentTask));
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
                InvokeCurrentCycle();
                PostCurrentCycle();
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
