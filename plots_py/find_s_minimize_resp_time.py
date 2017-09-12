import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


values = np.genfromtxt("../simulation_results/s_min_resp_time.csv", delimiter=";")
s_vect = values[:, 0]
n_vect = values[:, 1]
resp_vect = values[:, 2]

print(s_vect)
print(n_vect)
print(resp_vect)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_xlabel('S')
ax.set_ylabel('N')
ax.set_zlabel('Response Time')
ax.scatter(s_vect, n_vect, resp_vect)

ax.plot_wireframe(s_vect, n_vect, resp_vect, rstride=10, cstride=10)


plt.show()