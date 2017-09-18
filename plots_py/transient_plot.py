import numpy as np
import matplotlib.pyplot as plt
import subprocess
import random
import matplotlib.patches as mpatches

N = 20
simulation_num = 3

len1, len2 = 0, 0
seed1, seed2, seed3 = 12345, 12345, 12345

def generate_values(n):
    global len1, len2
    global seed1, seed2, seed3
    for s in range(n):
        if s == 0:
            subprocess.check_output(["../PMCSN", str(N), str(s), '1', '512', str(seed1)]) # only 1 batch to test behavior
            len1 = len(np.genfromtxt("../simulation_results/transient.csv", delimiter=";"))
            seed2 = seed1 + int(random.random() * 10000)
        elif s == 1:
            subprocess.check_output(["../PMCSN", str(N), str(s), '1', '512', str(seed2)]) # only 1 batch to test behavior
            len2 = len(np.genfromtxt("../simulation_results/transient.csv", delimiter=";"))
            seed3 = seed2 + int(random.random() * 10000)
        else:
            subprocess.check_output(["../PMCSN", str(N), str(s), '1', '512', str(seed3)]) # only 1 batch to test behavior




generate_values(simulation_num)

# load data from file
a = np.genfromtxt("../simulation_results/transient.csv", delimiter=";")

rt_mean1 = np.mean(a[:len1, 1])
transient_time1 = np.full((len(a[:len1, 1]), 1), rt_mean1)
rt_mean2 = np.mean(a[len1:len2, 1])
transient_time2 = np.full((len(a[len1:len2, 1]), 1), rt_mean2)
rt_mean3 = np.mean(a[len2:, 1])
transient_time3 = np.full((len(a[len2:, 1]), 1), rt_mean3)

rt_mean = (rt_mean1+rt_mean2+rt_mean3) / 3
tot_t_t = np.full((len(a[:, 1]), 1), rt_mean)

plt.axis([0, max(a[:, 0]), 0, max(a[:, 1]) + 0.5])
plt.xlabel("time")
plt.ylabel("Mean response time E[T]")
plt.plot(a[:len1, 0], a[:len1, 1], color='#2272f4', linewidth=2.0)
plt.plot(a[len1:len2, 0], a[len1:len2, 1], color='#11c61a', linewidth=2.0)
plt.plot(a[len2:, 0], a[len2:, 1], color='#a0139b', linewidth=2.0)
plt.plot(a[:, 0], tot_t_t, 'grey', linewidth=1.0)

a_patch = mpatches.Patch(color='#2272f4', label='Seed '+str(seed1))
b_patch = mpatches.Patch(color='#11c61a', label='Seed '+str(seed2))
c_patch = mpatches.Patch(color='#a0139b', label='Seed '+str(seed3))
plt.legend(handles=[a_patch, b_patch, c_patch])

plt.show()