import numpy as np
import matplotlib.pyplot as plt
import subprocess
import random


# varies S=N to write on file simulation results
def generate_data_from_simulation(n):
    seed = 1234
    for x in range(1, n + 1):
        subprocess.check_output(["../PMCSN", str(x), str(x), "15", "512", str(seed)])
        seed += random.random() * 100 # seed changes


generate_data_from_simulation(20)



# load data from file
a = np.genfromtxt("../simulation_results/res.csv", delimiter=";")
print(a[:, 1])

s = len(a) - 1
resp_time_min = min(a[:, 1]) - min(a[:, 1]) / 4
resp_time_max = max(a[:, 1]) + max(a[:, 1]) / 4

throughput_min = min(a[:, 3]) - min(a[:, 3]) / 4
throughput_max = max(a[:, 3]) + max(a[:, 3]) / 4

plt.figure(1)

x_err = (a[:, 1]) / 5
# first plot of response time vs S=N
plt.subplot(211)
plt.axis([0, s, resp_time_min, resp_time_max])
plt.errorbar(np.arange(s + 1), a[:, 1], xerr=0, yerr=a[:, 2])
plt.title("Response time")
plt.plot(a[:, 1], "bo")
plt.plot(a[:, 1], "k")
plt.ylabel("Time")

# second plot of throughput vs S=N
plt.subplot(212)
plt.axis([0, s, throughput_min, throughput_max])
plt.errorbar(np.arange(s + 1), a[:, 3], xerr=0, yerr=a[:, 4])
plt.title("Throughput")
plt.plot(a[:, 3], "ro")
plt.plot(a[:, 3], "k")
plt.xlabel("S Threshold")
plt.ylabel("Time")


plt.show()
