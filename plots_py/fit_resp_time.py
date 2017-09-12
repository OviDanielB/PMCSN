import numpy as np
import matplotlib.pyplot as plt
import scipy.stats as stats
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

#generate_data(80)


# load data from file
a = np.genfromtxt("../simulation_results/res.csv", delimiter=";")
print(a)
plt.hist(a[:, 1], bins=20, normed=True)

lognorm = stats.lognorm
x = np.linspace(0, 80, 1)
param = lognorm.fit(a[:, 1], floc=0)
print(param)
pdf_fitted = lognorm.pdf(x, *param)
plt.plot(x, pdf_fitted, 'ro')

#plt.plot(np.arange(0, 80, 1), a[:, 1])
plt.show()


