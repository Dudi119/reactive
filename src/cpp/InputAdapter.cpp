#include "InputAdapter.h"
#include <atomic>
#include "core/Exception.h"
#include "Event.h"
#include "UnitNode.h"
#include "GraphEngine.h"

namespace reactive{

    InputAdapter::InputAdapter():m_id(-1), m_isTicked(false), m_consumeType(ConsumeType::Non_Collapse){}
    InputAdapter::InputAdapter(int id):m_id(id), m_isTicked(false), m_consumeType(ConsumeType::Non_Collapse){}
    InputAdapter::~InputAdapter() {}

    void InputAdapter::PreStart(){}
    void InputAdapter::PostStart(){}

    void InputAdapter::PostStep()
    {
        GraphNode::PostStep();
        m_isTicked = false;
    }

    void InputAdapter::PostInvoke()
    {
        GraphNode::PostStep();
        m_isTicked = false;
    }

    void InputAdapter::PostStop() {}

    void InputAdapter::Invoke()
    {
        throw core::Exception(__CORE_SOURCE, "None supported operation");
    }

    void InputAdapter::AddConsumer(UnitNode &consumer)
    {
        m_consumers.emplace_back(consumer);
    }

    bool InputAdapter::ConsumeEvent(std::unique_ptr<Event> &&event)
    {
        if(m_consumeType == ConsumeType::Non_Collapse && m_isTicked == true)
            return false;

        m_event = std::move(event);
        if(m_isTicked == false)
        {
            m_isTicked = true;
            for( auto& consumer : m_consumers )
            {
                consumer.get().MarkInputEdge(m_id);
                GraphEngine::Instance().AddNodeToCycle(static_cast<GraphNode&>(consumer));
            }
        }
        return true;
    }

    sweetPy::object_ptr InputAdapter::GetLastData() const
    {
        if(m_event)
            return m_event->GetData();
        else
        {
            Py_XINCREF(Py_None);
            return sweetPy::object_ptr(Py_None, &sweetPy::Deleter::Owner);
        }
    }

    int InputAdapter::GenerateId()
    {
        static std::atomic_int id(0);
        return id++;
    }
}