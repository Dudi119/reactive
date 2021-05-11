#pragma once

#include <memory>
#include "ConcurrentFunction.h"
#include "core/SyncSharedQueue.h"

namespace reactive{
    class ConcurrentOrchestrator
    {
    public:
        static const int TASK_SIZE = 1024;
        typedef ConcurrentFunction<TASK_SIZE> ConcurrentTask;
        typedef ConcurrentTask::Pointer PointerType;
        typedef ConcurrentTask ValueType;

        ConcurrentOrchestrator(bool isSharedQueueOwner = false);
        void Dispatch(ValueType&& task);
        PointerType Pop();

    private:
        static constexpr int OUTGOING_QUEUE_SIZE = 5;
        core::SyncSharedQueue<ConcurrentTask, OUTGOING_QUEUE_SIZE>  m_outgoingQueue;
    };
}