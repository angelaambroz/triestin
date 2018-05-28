#!/usr/bin/python
# -*- coding: latin1 -*-
"""Tokenizer
"""

import re
from collections import namedtuple

Token = namedtuple("Token", ("tipo", "val"))


class Tokenizer():
    def __init__(self, source_code=None, filename=None):
        # Our language tokens (ordered tuple)
        self.TOKEN_TYPES = (
            ('print', r'\bdimmi\b'),
            ('def', r'\becco\b'),
            ('fin', r'\bfin\b'),
            ('anzi', r'\banzi\b'),
            ('identifier', r'\b[a-zA-Z]+\b'),
            ('assignment', u'è'),
            ('integer', r'\b[0-9]+\b'),
            ('math', r'[\+\-\/\*+]'),
            ('oparen', r'\('),
            ('cparen', r'\)'),
            ('comma', r'\,')
        )

        # Source code, or...
        if filename:
            with open(filename, 'r') as f:
                self.code = f.read().replace('\n', ' ')

        # ...REPL
        if source_code:
            self.code = source_code

    def tokenize(self):
        tokens = []

        while len(self.code) > 0:
            tokens.append(self.tokenize_one_token())

        if not tokens:
            raise RuntimeError(u"Non so proprio cos'è questo...")

        return tokens

    def tokenize_one_token(self):
        for (tipo, rgx_) in self.TOKEN_TYPES:
            rgx_ = r'^' + rgx_
            z = re.findall(rgx_, self.code)
            if z:
                self.code = self.code[len(z[0]):].strip()
                return Token(tipo, z[0])
