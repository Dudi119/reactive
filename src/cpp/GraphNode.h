#pragma once

#include <unordered_map>
#include <memory>

namespace reactive{

    class InputAdapter;

    class GraphNode
    {
    public:
        GraphNode():m_isTicked(false), m_executed(false){};
        virtual ~GraphNode(){}
        virtual void PreStart() = 0;
        virtual void PostStart() = 0;
        virtual void PostStep(){ m_isTicked = false; m_executed = false; };
        virtual void PostInvoke(){ m_isTicked = false; m_executed = false; };
        virtual void PostStop() = 0;
        virtual void Invoke() = 0;
        bool IsTicked() const {return m_isTicked;};
        void SetTicked() { m_isTicked = true; }
        bool IsExecuted() const {return m_executed;};
        void SetExecuted() { m_executed = true; }

    private:
        bool m_isTicked;
        bool m_executed;
    };

}