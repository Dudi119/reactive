#include "Executor.h"

#include <functional>

namespace reactive{
    Executor& Executor::Instance()
    {
        static Executor instance;
        return instance;
    }

    Executor::Executor()
    : m_isStopped(false)
    {
    }

    Executor::~Executor()
    {
        Stop();
    }

    void Executor::Start()
    {
        m_eventLoopThread.reset(new core::Thread("Executor Event loop thread", std::bind(&Executor::EventLoop, this)));
        std::mutex mut;
        std::unique_lock<std::mutex> guard(mut);
        {
            m_waitCv.wait(guard, []{return false;});
        }
    }

    void Executor::Stop()
    {
        if(m_isStopped == true)
            return;
        m_isStopped = true;
        ConcurrentOrchestrator::PointerType stubElement = MakeConcurrentFunction<ConcurrentOrchestrator::TASK_SIZE>(std::string());
        m_tasksOrchestrator.Dispatch(std::move(*stubElement));
        m_eventLoopThread->join();
    }

    void Executor::EventLoop()
    {
        while(m_isStopped == false)
        {
            ConcurrentOrchestrator::PointerType element = m_tasksOrchestrator.Pop();
            int len = element->GetFunction().length();
            element.release(); //We don't really want to take ownership on shared memory and to release it.
        }
    }
}

int main(int argc, char **argv) {
    reactive::Executor::Instance().Start();
}