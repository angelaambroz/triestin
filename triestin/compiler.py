#!/usr/bin/python
# -*- coding: latin1 -*-
"""Compiler
"""

from .parser import DefNode, IntegerNode, CallNode, VarRefNode, PrintNode, MathNode, AssignNode, AnziNode
from .state import State


class Generator():
    def __init__(self):
        return None

    def generate(self, node):
        """The glory and drama of exec() vs. eval() vs. ...compile()?
        """
        if type(node) == DefNode:
            return "def {}({}):\n return {}\n".format(node.name, 
                ','.join(node.arg_names), self.generate(node.body)), 'exec'
        if type(node) == CallNode:
            return "{}({})".format(node.name, 
                ','.join([self.generate(expr)[0] for expr in node.arg_exprs])), 'eval'
        if type(node) == AssignNode:
            return "{} = {}".format(node.name, node.val), 'exec'
        if type(node) == VarRefNode:
            return node.val, 'eval'
        if type(node) == IntegerNode:
            return str(node.val), 'eval'
        if type(node) == PrintNode:
            return "print('{}')".format(node.val), 'exec'
        if type(node) == MathNode:
            return "{}".format(node.val), 'eval'
        else:
            raise RuntimeError("Nodo tipo inaspettato {}".format(node))


