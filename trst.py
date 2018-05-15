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
        return self.parse_integer()

    def parse_integer(self):
        return IntegerNode(self.consume('integer').val)

    def consume(self, expected_type):
        token = self.tokens.pop(0)
        if token.tipo == expected_type:
            return token
        else:
            raise RuntimeError(f"Expected {expected_type}, got {token.tipo}.")

    def peek(self, expected_type):
        return self.tokens[0].tipo == expected_type

tree = Parser(tokens).parse()
print(tree)

# Code generator

# Compile to C, our definition


