import sys
from os.path import dirname, join
sys.path.append(join(dirname(__file__), '../../bin/'))
import _pyNode
from datetime import timedelta

class EdgeType(type):
    def __getitem__(cls, item):
        return Edge(item)

class Edge(metaclass=EdgeType):
    __all__ = ['type']
    def __init__(self, type):
        self.type = type

def idGenerator():
    return _pyNode.InputAdapter.generateId()

class OutDescriptorCreator(type):
    def __call__(cls, *args, **kwargs):
        if cls is OutDescriptor:
            return super(OutDescriptorCreator, cls).__call__(*args, **kwargs)
        else:
            edgeType = None
            for arg, (_, value) in zip(args, cls._signature.items()):
                if type(value) == Edge and (type(arg) == value.type or value.type == 'T'):
                    edgeType = type(arg)
                elif type(value) == Edge and (type(arg) == value.type or value.type == 'IT'):
                    _arg = cls.unpackContainer(arg)
                    edgeType = type(_arg)
                else:
                    cls.validateScalar(arg, value)

            if edgeType == None:
                raise TypeError('OutDescriptor is missing edge type')

            instance = super(OutDescriptorCreator, cls).__call__(idGenerator(), '',  edgeType)
            setattr(instance, '_args', args)
            setattr(instance, '_kwargs', kwargs)
            setattr(instance, 'inScore', 0)
            setattr(instance, 'nativeNode', instance.create(instance.id))
            GraphEngine().addNode(instance)
            return instance

    def unpackContainer(cls, arg):
        while cls.isContainer(arg):
            assert len(arg)
            arg = arg[0]
        return arg

    def validateScalar(cls, arg, schemeArg):
        if arg == None:
            return

        while cls.isContainer(arg):
            if type(arg) != type(schemeArg):
                raise TypeError('mismatch between OutDescriptor parameters - {0} is not instance of {1}'.format(type(arg), type(schemeArg)))
            if len(arg) == 0:
                return
            arg = arg[0]
            schemeArg = schemeArg[0]

        if type(arg) != schemeArg:
            raise TypeError('mismatch between OutDescriptor parameters - {0} is not instance of {1}'.format(type(arg), type(schemeArg)))

    def isContainer(cls, arg):
        return isinstance(arg, list) or isinstance(arg, tuple) or isinstance(arg, dict)

class OutDescriptorsTuple( tuple ):
    pass

class OutDescriptor(metaclass=OutDescriptorCreator):
    def __init__(self, id : int, name : str, type, node = None):
        self._id = id
        self._name = name
        self._type = type
        self._node = node #owener node
        self._consumers = []

    def setType(self, type):
        if self._type == None:
            self._type = type
        elif self._type != type:
            raise ValueError('Inconsistency with edge data type, new declared type - {0}, original - {1}'.format(type, self._type))

    def getType(self):
        return self._type

    def addConsumer(self, consumer):
        consumer.inScore += 1
        self._consumers.append(consumer)

    def create(self):
        pass

    @property
    def id(self):
        return self._id

    @property
    def node(self):
        return self._node

    @property
    def consumers(self):
        return self._consumers

    type = property(getType, setType)

class Signature(dict):
    pass

def makeOutDescriptor(name, signature, factory):
    def create(self, id):
        return type(self)._factory.create(*((id,) + self._args), **self._kwargs)

    cls = type(name,(OutDescriptor,), {'_factory':factory, '_signature':signature})
    setattr(cls, 'create', create)
    return cls


const = makeOutDescriptor('const', Signature(value=Edge['T']), _pyNode.ConstNodeFactory)
curve = makeOutDescriptor('curve', Signature(value=Edge['IT'], delta=[timedelta]), _pyNode.CurveNodeFactory)

def __Output__():
    pass

from graph_engine import GraphEngine
