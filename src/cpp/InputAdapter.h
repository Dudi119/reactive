#pragma once

#include <list>
#include <functional>
#include "GraphNode.h"

namespace reactive{

    class UnitNode;

    class InputAdapter : public GraphNode
    {
    public:
        virtual ~InputAdapter();
        void PreStart() override;
        void PostStart() override;
        void PostStep() override;
        void PostStop() override;
        void AddConsumer(UnitNode& consumer);

    private:
       std::list<std::reference_wrapper<UnitNode>> m_consumers;
    };
}