import matplotlib
import numpy as np
import matplotlib.pyplot as plt
import subprocess
import random
import math

from matplotlib.ticker import MaxNLocator
from mpl_toolkits.mplot3d import Axes3D

N = 20

def generate_values(N):
    seed = 1234
    for s in range(N + 1):
        subprocess.check_output(["../PMCSN", str(N), str(s), '15', '512', str(seed)])
        seed = seed + random.random() * 100



generate_values(N)


values = np.genfromtxt("../simulation_results/s_min_resp_time.csv", delimiter=";")
s_vect = values[:, 0]
s_int = []
# s_vect, labels = plt.yticks()
for each in s_vect:
    s_int.append(int(each))
plt.xticks(s_int)

resp_vect = values[:, 2]

plt.figure(1)
plt.xlabel("S")
plt.ylabel("Response Time")
plt.title("Minimum Response time achieved in function of S (N=20)")
plt.grid(True)
plt.plot(s_vect, resp_vect, 'r', linewidth=3.0)


plt.show()