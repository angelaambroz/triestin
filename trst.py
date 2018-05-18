#!/usr/bin/python

import argparse
from triestin import Tokenizer, Parser, Generator, State

def triestin(text):
    tokens = Tokenizer(source_code=text).tokenize()
    tree = Parser(tokens).parse()
    return Generator().generate(tree)

if __name__=="__main__":

    local_state = State()
    print("Triestin v0.0.1")
    print("Premi ctrl+C per uscire")

    while True:
        user_input = raw_input('triestin> ')
        local_state.add_history(str(user_input))
        exec(triestin(user_input))


