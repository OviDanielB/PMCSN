import numpy as np
import matplotlib.pyplot as plt



# load data from file
a = np.genfromtxt("../simulation_results/res.csv", delimiter=";")
print(a[:, 1])

s = len(a) - 1
resp_time_min = min(a[:, 1]) - min(a[:, 1]) / 4
resp_time_max = max(a[:, 1]) + max(a[:, 1]) / 4

throughput_min = min(a[:, 2]) - min(a[:, 2]) / 4
throughput_max = max(a[:, 2]) + max(a[:, 2]) / 4

plt.figure(1)

x_err = (a[:, 1]) / 5
# first plot
plt.subplot(211)
plt.axis([0, s, resp_time_min, resp_time_max])
plt.errorbar(np.arange(s + 1), a[:, 1], xerr=0, yerr=a[:, 2])
plt.title("Response time")
plt.plot(a[:, 1], "bo")
plt.plot(a[:, 1], "k")
plt.ylabel("Time")

# second plot
plt.subplot(212)
plt.axis([0, s, throughput_min, throughput_max])
plt.errorbar(np.arange(s + 1), a[:, 3], xerr=0, yerr=a[:, 4])
plt.title("Throughput")
plt.plot(a[:, 2], "ro")
plt.plot(a[:, 2], "k")
plt.xlabel("S Threshold")
plt.ylabel("Time")


plt.show()
