import sys
from os.path import dirname, join

sys.path.append(join(dirname(__file__), '../bin/'))
sys.path.append(join(dirname(__file__), '../src/python/'))
from unit import unit
from wiring import Edge, __Output__
from graph import sub_graph, graph
import _core
from _engine import GraphEngine


@unit
def unitA():
    __Outputs__(a = str)
    x = 5
    print(x)
    return x

@unit
def unitB(scalarInt, inputA = Edge[int]):
    print(scalarInt)

@sub_graph
def sub_graph_a():
    x, y = unitA()
    unitB(1, y)
    return y

@graph
def main():
    y = sub_graph_a()

class Logger:
    def __enter__(self):
        _core.Logger.instance().start(_core.Severity.Info)

    def __exit__(self, exc_type, exc_val, exc_tb):
        _core.Logger.instance().terminate()

if __name__ == "__main__":
    with Logger():
        _core.Environment.instance().init()
        main()
        GraphEngine.instance().stop()


