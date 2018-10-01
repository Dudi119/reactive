class EdgeType(type):
    def __getitem__(cls, item):
        return Edge(item)

class Edge(metaclass=EdgeType):
    __class__ = EdgeType
    __all__ = ['type']
    def __init__(self, instance):
        self.type = type(instance)
