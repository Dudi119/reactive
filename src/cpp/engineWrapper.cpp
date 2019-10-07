#include "sweetPy/sweetPy.h"
#include "GraphEngine.h"
#include "InputAdapter.h"
#include "UnitNode.h"

namespace reactive{
    INIT_MODULE(_engine, "reactive graph engine")
    {
        sweetPy::Clazz<GraphEngine> engine(module, "GraphEngine", "The graph engine");
        engine.add_static_method("instance", "Provides access to the GraphEngine instance", &GraphEngine::Instance);
        engine.add_method("stop", "stops the engine", &GraphEngine::Stop);
        engine.add_method("start", "initiates the engine run", &GraphEngine::Start);
    }
}
