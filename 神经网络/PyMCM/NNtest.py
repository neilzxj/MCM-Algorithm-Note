# -*- coding: utf-8 -*-
from pybrain.structure import *
from pybrain.supervised.trainers import *
from pybrain.datasets import *
from pybrain.tools.shortcuts import buildNetwork

n = buildNetwork(2, 3, 1,bias=True,hiddenclass=TanhLayer)

ds = SupervisedDataSet(2, 1)
ds.addSample([0, 0], [0])
ds.addSample([0, 1], [1])
ds.addSample([1, 0], [1])
ds.addSample([1, 1], [0])

print " begin BP"
trainer = BackpropTrainer(n, ds, verbose=True, learningrate=0.01)
print "begin training"
for i in xrange(1100):
    trainer.train()

print "return results"
print n.activate([0, 0])
print n.activate([0, 1])
print n.activate([1, 0])
print n.activate([1, 1])
