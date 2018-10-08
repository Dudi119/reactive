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
        self._nodes.sort(key = lambda elem : elem.inScore)
        roots = []
        nodeToNative = {}
        for node in self._nodes:
            if node.inScore == 0:
                roots.append(node)
            nodeToNative[node] = node.create()

        while roots:
            currentRoot = roots[0]
            for outDescriptor in currentRoot.outDescriptors:
                for consumer in outDescriptor.consumers:
                    consumer.inScore -= 1
                    if consumer.inScore == 0:
                        roots.append(consumer)
            roots.pop(0)
