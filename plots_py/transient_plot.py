import numpy as np
import matplotlib.pyplot as plt


# load data from file
a = np.genfromtxt("../simulation_results/transient.csv", delimiter=";")


rt_mean = np.mean(a[:, 1])
transient_time = np.full((len(a[:, 1]), 1), rt_mean)


plt.axis([0, max(a[:, 0]), 0, max(a[:, 1]) + 0.5])
plt.xlabel("time")
plt.ylabel("Mean response time E[T]")
plt.plot(a[:, 0], a[:, 1], linewidth=2.0)
plt.plot(a[:, 0], transient_time, 'r', linewidth=2.0)
plt.show()