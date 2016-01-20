import matplotlib.pyplot as plt
import numpy as np
mean=[0,0]
cov=[[-50,50],[0,1]]

x, y = np.random.multivariate_normal(mean, cov, 500).T
plt.plot(x, y, '*')
plt.axis('equal')
plt.show()
