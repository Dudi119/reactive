#include <iostream>
#include "gtest/gtest.h"
#include "sweetPy/sweetPy.h"
#include "src/cpp/GraphEngine.h"
#include "src/cpp/ConstNode.h"

static int _argc;
static char **_argv;

namespace reactive_tests{
class ReactiveUnitTestEnvironment : public ::testing::Environment {
    public:
        ReactiveUnitTestEnvironment() {}

        void SetUp() override {
            core::Logger::Instance().Start(core::TraceSeverity::Info);
            Py_Initialize();
        }

        void TearDown() override {
            Py_Finalize();
        }
    };

    TEST(ReactiveTests, Something) {
    //reactive::GraphEngine& engine = reactive::GraphEngine::Instance();
        PyObject* val = sweetPy::Object<int>::to_python(5);
        sweetPy::ObjectPtr trackedVal(val, &sweetPy::Deleter::Owner);
        sweetPy::TimeDelta timeDelta(1, 0, 0);
        reactive::InputAdapter& constAdapter = reactive::ConstNodeFactory::Create(reactive::InputAdapter::GenerateId(),
            trackedVal.get(), timeDelta);

        reactive::GraphEngine::Instance().Start(sweetPy::TimeDelta(3, 0, 0));
        reactive::GraphEngine::Instance().Stop();
    }
}

int main(int argc, char **argv) {
    _argc = argc;
    _argv = argv;
    ::testing::AddGlobalTestEnvironment(new reactive_tests::ReactiveUnitTestEnvironment());
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}