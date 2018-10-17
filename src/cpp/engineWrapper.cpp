#include "sweetPy/InitModule.h"
#include "sweetPy/CPythonClass.h"
#include "sweetPy/CPythonEnum.h"
#include "GraphEngine.h"
#include "UnitNode.h"

namespace reactive{
    INIT_MODULE(_engine, "reactive graph engine")
    {
        sweetPy::CPythonClass<GraphEngine> engine(module, "GraphEngine", "The graph engine");
        engine.AddStaticMethod("instance", "Provides access to the GraphEngine instance", &GraphEngine::Instance);
        engine.AddMethod("stop", "stops the engine", &GraphEngine::Stop);
    }
}
