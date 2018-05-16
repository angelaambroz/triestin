"""Tokenizer
"""

import re 
from collections import namedtuple

Token = namedtuple("Token", ("tipo", "val"))

class Tokenizer():
    def __init__(self, filename):
        self.file = filename

        # defining our language tokens
        # order matters! hence i'm using a tuple
        self.TOKEN_TYPES = (
            ('print', r'\bdimmi\b'),
            ('def', r'\becco\b'), 
            ('fin', r'\bfin\b'),
            ('identifier', r'\b[a-zA-Z]+\b'),
            ('integer', r'\b[0-9]+\b'),
            ('+', r'\+'),
            ('-', r'-'),
            ('/', r'/'),
            ('*', r'\*'),
            ('**', r'\*\*'),
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

