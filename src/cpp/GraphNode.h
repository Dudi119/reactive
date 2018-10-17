#pragma once

namespace reactive{

    class GraphNode
    {
    public:
        virtual ~GraphNode(){}
        virtual void PreEngineStart() = 0;
        virtual void PostEngineStart() = 0;
        virtual void PostEngineStep() = 0;
        virtual void Stop() = 0;
    };

}