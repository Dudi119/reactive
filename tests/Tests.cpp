#include <iostream>
#include "gtest/gtest.h"
#include "sweetPy/sweetPy.h"
#include "src/cpp/GraphEngine.h"
#include "src/cpp/ConstNode.h"
#include "AsserterNode.h"

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
        PyObject* val = sweetPy::Object<int>::to_python(5);
        sweetPy::ObjectPtr trackedVal(val, &sweetPy::Deleter::Owner);
        sweetPy::TimeDelta timeDelta(1, 0, 0);
        reactive::InputAdapter& constAdapter = reactive::ConstNodeFactory::Create(reactive::InputAdapter::GenerateId(),
            trackedVal.get(), timeDelta);

        std::function<void(int)> assertionFunction = [](int val) {
            ASSERT_EQ(val, 5);
        };
        std::array<std::reference_wrapper<reactive::InputAdapter>, 1> arguments = {constAdapter};
        auto asserterNode = AsserterNodeFactory::Create(assertionFunction, arguments);
        constAdapter.AddConsumer(*asserterNode);

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