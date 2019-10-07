#include "sweetPy/sweetPy.h"
#include "core/Logger.h"
#include "core/Environment.h"

namespace reactive{
    INIT_MODULE(_core, "Core library")
    {
        sweetPy::Clazz<core::Environment> environment(module, "Environment", "Core's Envriornment handler");
        environment.add_static_method("instance", "Provides access to the Environment instance", &core::Environment::Instance);
        environment.add_method("init", "Will initialize the environemnt instance", &core::Environment::Init);

        sweetPy::Clazz<core::Logger> logger(module, "Logger", "Logger");
        logger.add_static_method("instance", "Provides access to the Logger instance", &core::Logger::Instance);
        logger.add_method("start", "Will start the logger", &core::Logger::Start);
        logger.add_method("terminate", "Will terminate the logger", &core::Logger::Terminate);

        sweetPy::Enum severity(module, "Severity");
        severity.add_value("Info", core::Info);
        severity.add_value("Verbose", core::Verbose);
        severity.add_value("Fatal", core::Fatal);
    }
}

