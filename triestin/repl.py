"""Trying to manage history, argh
"""

class State(object):
    """docstring for State"""
    def __init__(self):
        self.history = []

    def add_history(self, text):
        """Add commands in order"""
        self.history.append(text)
        
