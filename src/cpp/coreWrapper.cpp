#include "sweetPy/InitModule.h"
#include "sweetPy/CPythonClass.h"
#include "sweetPy/CPythonEnum.h"
#include "core/Logger.h"
#include "core/Environment.h"

namespace reactive{
    INIT_MODULE(_core, "Core library")
    {
        sweetPy::CPythonClass<core::Environment> environment(module, "Environment", "Core's Envriornment handler");
        environment.AddStaticMethod("instance", "Provides access to the Environment instance", &core::Environment::Instance);
        environment.AddMethod("init", "Will initialize the environemnt instance", &core::Environment::Init);

        sweetPy::CPythonClass<core::Logger> logger(module, "Logger", "Logger");
        logger.AddStaticMethod("instance", "Provides access to the Logger instance", &core::Logger::Instance);
        logger.AddMethod("start", "Will start the logger", &core::Logger::Start);
        logger.AddMethod("terminate", "Will terminate the logger", &core::Logger::Terminate);

        sweetPy::CPythonEnum severity(module, "Severity");
        severity.AddEnumValue("Info", core::Info);
        severity.AddEnumValue("Verbose", core::Verbose);
        severity.AddEnumValue("Fatal", core::Fatal);
    }
}

