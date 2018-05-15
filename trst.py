#!/usr/bin/python3 

import re

# Tokenizer/Lexer
class Token():
    def __init__(self, tipo, val):
        self.tipo = tipo
        self.val = val

    def __str__(self):
        return f"Token(tipo='{self.tipo}', val='{self.val}')"

class Tokenizer():
    def __init__(self, filename):
        self.file = filename

        # defining our language tokens
        # order matters! hence i'm using a tuple
        self.TOKEN_TYPES = (
            ('def', r'\bdef\b'), 
            ('fin', r'\bfin\b'),
            ('identifier', r'\b[a-zA-Z]+\b'),
            ('integer', r'\b[0-9]+\b'),
            ('oparen', r'\('),
            ('cparen', r'\)'),
            ('comma', r'\,')
        )
        
        # the user-sourced code
        with open(filename, 'r') as f:
            self.code = f.read().replace('\n', '')
        
    def tokenize(self):
        tokens = []
        
        while len(self.code) > 0:
            tokens.append(self.tokenize_one_token())

        return tokens
                    
    def tokenize_one_token(self):
        for (tipo, rgx_) in self.TOKEN_TYPES:
            rgx_= r'^' + rgx_
            z = re.findall(rgx_, self.code)
            if z:
                self.code = self.code[len(z[0]):].strip()
                return Token(tipo, z[0])

tokens = Tokenizer("trst.src").tokenize()
for i in tokens:
    print(i)

# Parser
class DefNode():
    def __init__(self, name, arg_names, body):
        self.name = name
        self.arg_names = arg_names
        self.body = body

    def __str__(self):
        return f"DefNode(name='{self.name}', args='{self.arg_names}', body='{self.body}'"

class IntegerNode():
    def __init__(self, val):
        self.val = int(val)

    def __str__(self):
        return f"IntegerNode(val={self.val})"

class CallNode():
    def __init__(self, name, arg_exprs):
        self.name = name
        self.arg_exprs = arg_exprs

    def __str__(self):
        return f"CallNode(name={self.name.val}, arg_exprs={self.arg_exprs})"

class VarRefNode():
    def __init__(self, val):
        self.val = val

    def __str__(self):
        return f"VarRefNode(val={self.val}"

class Parser():
    def __init__(self, list_of_tokens):
        self.tokens = list_of_tokens

    def parse(self):
        return self.parse_def()

    def parse_def(self):
        self.consume('def')
        name = self.consume('identifier').val
        arg_names = self.parse_arg_names()
        body = self.parse_expr()
        self.consume('fin')
        return DefNode(name, arg_names, body)

    def parse_arg_names(self):
        self.consume('oparen')
        # stuff in here
        args = []
        if self.peek('identifier'):
            args.append(self.consume('identifier').val)
            while self.peek('comma'):
                self.consume('comma')
                args.append(self.consume('identifier').val)
        self.consume('cparen')
        return args

    def parse_expr(self):
        if self.peek('integer'):
            return self.parse_integer()
        elif self.peek('identifier') and self.peek('oparen', 1):
            return self.parse_call()
        else:
            return self.parse_var_ref()

    def parse_integer(self):
        return IntegerNode(self.consume('integer').val)

    def parse_call(self):
        name = self.consume('identifier')
        arg_exprs = self.parse_arg_exprs()
        return CallNode(name, arg_exprs)

    def parse_arg_exprs(self):
        self.consume('oparen')
        # stuff in here
        arg_exprs = []
        if not self.peek('cparen'):
            arg_exprs.append(self.parse_expr())
            while self.peek('comma'):
                self.consume('comma')
                arg_exprs.append(self.parse_expr())
        self.consume('cparen')
        return arg_exprs

    def parse_var_ref(self):
        return VarRefNode(self.consume('identifier').val)

    def consume(self, expected_type):
        token = self.tokens.pop(0)
        if token.tipo == expected_type:
            return token
        else:
            raise RuntimeError(f"Expected {expected_type}, got {token.tipo}.")

    def peek(self, expected_type, offset=0):
        return self.tokens[offset].tipo == expected_type

tree = Parser(tokens).parse()
print(tree)

# Code generator
# Compile to C, our definition
class Generator():
    def __init__(self):
        return None

    def generate(self, node):
        if node:
            return node
        else:
            raise RuntimeError(f"Unexpected type {node}")

generated = Generator().generate(tree)
print(generated)

