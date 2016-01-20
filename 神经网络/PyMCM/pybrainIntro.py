# coding=utf-8
from pybrain.structure import FeedForwardNetwork
from pybrain.structure import LinearLayer, SigmoidLayer
from pybrain.structure import FullConnection
from pybrain.structure import RecurrentNetwork

n = FeedForwardNetwork()
# There are a couple of different classes of layers.
# For a complete list check out the modules package.
inLayer = LinearLayer(2)
hiddenLayer = SigmoidLayer(3)
outLayer = LinearLayer(1)

# In order to use them, we have to add them to the network:
n.addInputModule(inLayer)
n.addModule(hiddenLayer)
n.addOutputModule(outLayer)

# Produces a full connectivity between layers
in_to_hidden = FullConnection(inLayer, hiddenLayer)
hidden_to_out = FullConnection(hiddenLayer, outLayer)
n.addConnection(in_to_hidden)
n.addConnection(hidden_to_out)

# This call does some internal initialization
n.sortModules()

# Print some info inside of NN
print in_to_hidden.params
print hidden_to_out.params
print n.params
print n.activate([1, 2])

# Naming your NN
print LinearLayer(2).name
LinearLayer(2, name='foo')
print LinearLayer(2).name

# Using Recurrent NN
n = RecurrentNetwork()

n.addInputModule(LinearLayer(2, name='in'))
n.addModule(SigmoidLayer(3, name='hidden'))
n.addOutputModule(LinearLayer(1, name='out'))
n.addConnection(FullConnection(n['in'], n['hidden'], name='c1'))
n.addConnection(FullConnection(n['hidden'], n['out'], name='c2'))

# Looks back in time one timestep
n.addRecurrentConnection(FullConnection(n['hidden'], n['hidden'], name='c3'))

# Using RNN, every steps gets different value of Neron
n.sortModules()
print n.activate([2, 2])
print n.activate([2, 2])
print n.activate([2, 2])
n.reset()  # Clear n and Reset it
print n.activate([2, 2])
print n.activate([2, 2])
