"""Compiler
"""

from .parser import DefNode, IntegerNode, CallNode, VarRefNode, PrintNode

class Generator():
    def __init__(self):
        return None

    def generate(self, node):
        # case in Ruby is ???? in Python?
        if type(node) == DefNode:
            return "def {}({','.join()}):\n return {}\n".format(node.name, node.arg_names, self.generate(node.body))
        if type(node) == CallNode:
            return "{}({})".format(node.name, ','.join([self.generate(expr) for expr in node.arg_exprs]))
        if type(node) == VarRefNode:
            return node.val
        if type(node) == IntegerNode:
            return str(node.val)
        if type(node) == PrintNode:
            return "print('{}')".format(node.val)
        else:
            raise RuntimeError("Tipo inaspettato {}".format(node))