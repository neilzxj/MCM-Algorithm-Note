# coding=utf-8
import pybrain
from pybrain.supervised.trainers import BackpropTrainer
from pybrain.tools.shortcuts import buildNetwork
from pybrain.datasets import SupervisedDataSet

net = buildNetwork(2, 3, 1, bias=True, hiddenclass=pybrain.TanhLayer)
# 建立输入层为2,隐藏层为3,输出层为1的神经网络
ds = SupervisedDataSet(2, 1)
# 建立监督学习的数据训练集
trainer = BackpropTrainer(net, ds)
# 反向传播算法训练神经网络

ds.addSample((0, 0), (0,))
ds.addSample((0, 1), (1,))
ds.addSample((1, 0), (1,))
ds.addSample((1, 1), (0,))
# 向数据集里添加数据

print trainer.train()
# 一次训练的结果
print trainer.trainUntilConvergence()
# 返回一个tuple,是每次训练的结果,直到结果收敛为止
a=raw_input().split()