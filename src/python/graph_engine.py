from unit import UnitDescriptor
import utility

class GraphEngine(metaclass=utility.Singelton):
    __slots__ = ['_nodes']

    def __init__(self):
        self._nodes = []

    def clear(self):
        self._nodes = []

    def addNode(self, node : UnitDescriptor):
        self._nodes.append(node)

    def build(self):
        pass