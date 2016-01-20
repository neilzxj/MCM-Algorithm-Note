from random import random
from pybrain.structure.evolvables.evolvable import Evolvable
from pybrain.optimization import HillClimber


class SimpleEvo(Evolvable):
    def __init__(self, x): self.x = max(0, min(x, 10))

    def mutate(self):      self.x = max(0, min(self.x + random() - 0.3, 10))

    def copy(self):        return SimpleEvo(self.x)

    def randomize(self):   self.x = 10 * random()

    def __repr__(self):    return '<-%.2f->' + str(self.x)


x0 = SimpleEvo(5)
l = HillClimber(lambda x: x.x, x0, maxEvaluations=50)
print l.learn()
