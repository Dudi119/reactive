#pragma once

#include <memory>
#include <condition_variable>
#include "core/Thread.h"
#include "ConcurrentOrchestrator.h"

namespace reactive{
    class Executor
    {
    public:
        static Executor& Instance();
        ~Executor();
        void Stop();
        void Start();

    private:
        Executor();
        void EventLoop();

    private:
        bool m_isStopped;
        std::unique_ptr<core::Thread> m_eventLoopThread;
        std::condition_variable m_waitCv;
        ConcurrentOrchestrator m_tasksOrchestrator;
    };
}