import sys
from os.path import dirname, join

sys.path.append(join(dirname(__file__), '../../bin/'))
sys.path.append(join(dirname(__file__), '../src/python/'))
from unit import unit
from wiring import Edge


@unit
def unitA():
    return 5

@unit
def unitB(scalarInt, inputA = Edge[int]):
    pass

def main():
    unitB(1, unitA())

if __name__ == "__main__":
    main()

