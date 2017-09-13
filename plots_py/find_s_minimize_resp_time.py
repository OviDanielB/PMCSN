import numpy as np
import matplotlib.pyplot as plt
import subprocess
import random
from mpl_toolkits.mplot3d import Axes3D


def generate_values(n_max):
    seed = 1234
    for n in range(n_max + 1):
        for s in range(n + 1):
            subprocess.check_output(["../PMCSN", str(n), str(s), '10', '69', str(seed)])
            seed = seed + random.random() * 100



#generate_values(50)


values = np.genfromtxt("../simulation_results/s_min_resp_time.csv", delimiter=";")
s_vect = values[:, 0]
n_vect = values[:, 1]
resp_vect = values[:, 2]

n_max =  int(max(n_vect)) + 1

final_s_vect = np.arange(n_max)
#fianl_resp_vect = np.full((n_max, 1), 1000.)
curr_min_resp = np.full((n_max, 1), 1000.)

def find_s_min():
    for n in range(n_max):
        for v in values:
            if int(v[1]) == n:
                if v[2] < curr_min_resp[n, 0]:
                    final_s_vect[n] = int(v[0])
                    curr_min_resp[n, 0] = v[2]
    print(final_s_vect)
    print(curr_min_resp)


find_s_min()




plt.figure(1)
plt.xlabel("N")
plt.ylabel("S")
plt.title("S in function of N to minimize response time")
plt.grid(True)
plt.plot(np.arange(n_max), final_s_vect, 'g', linewidth=3.0)

plt.figure(2)
plt.xlabel("N")
plt.ylabel("Min Response Time")
plt.title("Minimum Response time achieved in function of N")
plt.grid(True)
plt.plot(np.arange(n_max), curr_min_resp,'r', linewidth=3.0)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_xlabel('S')
ax.set_ylabel('N')
ax.set_zlabel('Response Time')
ax.scatter(s_vect, n_vect, resp_vect)

#ax.plot_wireframe(s_vect, n_vect, resp_vect, rstride=10, cstride=10)


fig_min = plt.figure()
ax_min = fig_min.add_subplot(111, projection='3d')
ax_min.set_xlabel('S')
ax_min.set_ylabel('N')
ax_min.set_zlabel('Response Time')
ax_min.scatter(final_s_vect, np.arange(n_max), curr_min_resp, c='b', marker='o', depthshade=False)


plt.show()