#include "UnitNode.h"
#include "core/Exception.h"
#include "InputAdapter.h"

namespace reactive
{
    UnitNode::~UnitNode() {}

    void UnitNode::AddConsumer(int inputId, UnitNode& consumer)
    {
        if(m_inputAdapters.find(inputId) == m_inputAdapters.end())
            throw core::Exception(__CORE_SOURCE, "Invalid inputId - %d, input adapter dosn't exists", inputId);
        m_inputAdapters[inputId]->AddConsumer(consumer);
    }

    void UnitNode::PreEngineStart(){}
    void UnitNode::PostEngineStart(){}
    void UnitNode::PostEngineStep(){}
    void UnitNode::PostStop(){}
}
