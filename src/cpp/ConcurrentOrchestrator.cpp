#include "ConcurrentOrchestrator.h"

namespace reactive{
    ConcurrentOrchestrator::ConcurrentOrchestrator(bool isSharedQueueOwner)
    : m_outgoingQueue("ConcurrentOrchestratorOutgoingQueue", isSharedQueueOwner, core::SharedObject::AccessMod::READ_WRITE)
    {
    }

    void ConcurrentOrchestrator::Dispatch(ValueType&& task)
    {
        m_outgoingQueue.push(std::move(task));
    }

    typename ConcurrentOrchestrator::PointerType ConcurrentOrchestrator::Pop()
    {
        PointerType concurrentTask(MakeConcurrentFunction<TASK_SIZE>(std::string()));
        m_outgoingQueue.pop(*concurrentTask);
        return concurrentTask;
    }
}