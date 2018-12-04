#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <functional>
#include <bitset>
#include "Event.h"
#include "InputAdapter.h"
#include "GraphEngine.h"
#include "GraphNode.h"

namespace reactive {


    class UnitNode : public GraphNode
    {
    public:
        typedef int EdgeId;

        virtual ~UnitNode();
        void AddConsumer(EdgeId id, UnitNode& consumer);
        InputAdapter& GetOutEdge(EdgeId id);
        void PreStart() override;
        void PostStart() override;
        void PostInvoke() override;
        void PostStep() override;
        void PostStop() override;
        void MarkInputEdge(int edge);
        static void ProduceOutEdgeData(UnitNode& node, int outEdgeId, PyObject* data);

    protected:
        typedef int LogicalId;
        static const int MAX_INPUT_EDGES = 50;
        std::bitset<MAX_INPUT_EDGES> m_tickedInput;
        std::vector<std::reference_wrapper<InputAdapter>> m_inEdges;
        std::unordered_map<EdgeId, LogicalId> m_inEdgesMapping;
        std::unordered_map<EdgeId, InputAdapter> m_outEdges;
    };
}
