import sys
from os.path import dirname, join

sys.path.append(join(dirname(__file__), '../../bin/'))
sys.path.append(join(dirname(__file__), '../src/python/'))
from unit import unit
from wiring import Edge
from graph import sub_graph, graph



@unit
def unitA():
    __Outputs__(x = int, a = str)
    x = 5
    print(x)
    return x

@unit
def unitB(scalarInt, inputA = Edge[int]):
    print(scalarInt)

@sub_graph
def sub_graph_a():
    x, y = unitA()
    unitB(1, x)
    return y

@graph
def main():
    y = sub_graph_a()

if __name__ == "__main__":
    main()

