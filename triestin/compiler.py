"""Compiler
"""

from .parser import DefNode, IntegerNode, CallNode, VarRefNode, PrintNode

class Generator():
    def __init__(self):
        return None

    def generate(self, node):
        # case in Ruby is ???? in Python?
        if type(node) == DefNode:
            return f"def {node.name}({','.join(node.arg_names)}):\n return {self.generate(node.body)}\n"
        if type(node) == CallNode:
            return f"{node.name}({','.join([self.generate(expr) for expr in node.arg_exprs])})"
        if type(node) == VarRefNode:
            return node.val
        if type(node) == IntegerNode:
            return str(node.val)
        if type(node) == PrintNode:
            return f"print('{node.val}')"
        else:
            raise RuntimeError(f"Unexpected type {node}")