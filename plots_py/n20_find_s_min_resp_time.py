import matplotlib
import numpy as np
import matplotlib.pyplot as plt
import subprocess
import random
import math

from matplotlib.lines import lineMarkers
from mpl_toolkits.mplot3d import Axes3D

N = 20

def generate_values(N):
    seed = 1234
    for s in range(N + 1):
        subprocess.check_output(["../PMCSN", str(N), str(s), '15', '512', str(seed)])
        seed = seed + random.random() * 100



#generate_values(N)


values = np.genfromtxt("../simulation_results/s_min_resp_time.csv", delimiter=";")
s_vect = values[:, 0]
resp_vect = values[:, 2]

s_min = 0
r_min= resp_vect[0]
s_int = []
# s_vect, labels = plt.yticks()
for i in range(len(s_vect)):
    s_int.append(int(s_vect[i]))
    if r_min > resp_vect[i]:
        r_min = resp_vect[i]
        s_min = i

plt.xticks(s_int)

min_time = np.full((len(values[:, 0]), 1), r_min)



plt.figure(1)
plt.xlabel("S")
plt.ylabel("Response Time")
plt.title("Minimum Response time achieved in function of S (N=20)")
plt.grid(True)
plt.plot(s_vect, resp_vect, 'b', linewidth=3.0, linestyle='-.')
plt.plot(values[:, 0], min_time, 'r', linewidth=1.5)
plt.plot((s_min, s_min), (0, r_min), 'r', linewidth=1.5)

plt.show()