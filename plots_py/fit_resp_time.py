import numpy as np
import matplotlib.pyplot as plt
import os
import subprocess
import random

#os.remove("../simulation_results/res.csv")

# generates csv file with response time results
def generate_data(n):
    seed = 1234
    for x in range(n):
        output = subprocess.check_output(['../PMCSN', str(x), str(x), '100', '512', str(int(seed))])
        seed += random.random() * 100

#generate_data(50)


# load data from file
a = np.genfromtxt("../simulation_results/res.csv", delimiter=";")
#plt.hist(a[:, 1], bins=20)
plt.plot(np.arange(0, 50, 1), a[:, 1])
plt.show()


