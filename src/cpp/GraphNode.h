#pragma once

#include <unordered_map>
#include <memory>

namespace reactive{

    class InputAdapter;

    class GraphNode
    {
    public:
        virtual ~GraphNode(){}
        virtual void PreStart() = 0;
        virtual void PostStart() = 0;
        virtual void PostStep(){ m_isTicked = false; };
        virtual void PostStop() = 0;
        virtual void Invoke() = 0;
        bool IsTicked() const {return m_isTicked;};
        void SetTicked() { m_isTicked = true; }

    private:
        bool m_isTicked;
    };

}