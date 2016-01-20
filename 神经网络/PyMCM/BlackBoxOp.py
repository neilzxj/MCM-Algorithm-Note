from pybrain.optimization import CMAES
from numpy import array

def objF(x):
    return sum(x ** 2)

x0 = array([2.1, -1])
l = CMAES(objF, x0, minimize=True)
l.maxEvaluations=200
print l.learn()
