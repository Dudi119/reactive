import typing
from typing import Any
import types
import inspect

Function = typing.NewType('Function', types.FunctionType)

def runtime_check(func):

    def checker_wrapper(*args, **kwargs):
        hints = typing.get_type_hints(func)
        sig = inspect.signature(func)
        for arg, argName in zip(args, sig.parameters.values()):
            if argName in hints and not isinstance(arg, hints[argName]):
                raise TypeError('invalid type ')
        for argName in kwargs.keys():
            if argName in hints and not isinstance(kwargs[argName], hints[argName]):
                raise TypeError('invalid type ')
        return func(*args, **kwargs)

    return checker_wrapper
