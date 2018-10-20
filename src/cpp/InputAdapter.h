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
        void PreEngineStart() override;
        void PostEngineStart() override;
        void PostEngineStep() override;
        void PostStop() override;
        void AddConsumer(UnitNode& consumer);

    private:
       std::list<std::reference_wrapper<UnitNode>> m_consumers;
    };
}