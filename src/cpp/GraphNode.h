#pragma once

namespace reactive{

    class GraphNode
    {
    public:
        virtual ~GraphNode(){}
        virtual void PreStart() = 0;
        virtual void PostStart() = 0;
        virtual void PostStep() = 0;
        virtual void PostStop() = 0;
    };

}