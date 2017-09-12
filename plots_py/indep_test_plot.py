import numpy as np
import matplotlib.pyplot as plt

# load data from file
a = np.genfromtxt("../simulation_results/ind_test_res_n_60000.csv")
failures_n = np.genfromtxt("../simulation_results/ind_test_results.csv", delimiter=';')
print(failures_n)

v1 = a[0]  # lower critical value
v2 = a[1]  # upper critical value
chi_vector = a[2:]

x_vect = np.arange(0, len(chi_vector), 1)
v1_vect = np.full((len(x_vect), 1), v1)
v2_vect = np.full((len(x_vect), 1), v2)

count = 0
for chi in chi_vector:
    if v1 < chi < v2:
        count += 1

print("Failed test count : " + str(len(chi_vector) - count))
print("Given the confidence level of alpha = 0.05, " + str(0.05 * len(chi_vector)) + " should fail!")
print(count / len(chi_vector))

plt.figure(1)
plt.xlabel("Stream")
plt.ylabel("Chi Square Statistic")
plt.yticks([v1, v2], ["v1", "v2"])
plt.axis([0, len(chi_vector) + 1, min(chi_vector) - 1, max(chi_vector) + 1])
plt.plot(x_vect, chi_vector, 'bo')
plt.plot(x_vect, v1_vect, 'r-')
plt.plot(x_vect, v2_vect, 'r-')


plt.figure(2)
plt.axis([0, max(failures_n[:, 0]) + 10000, min(failures_n[:, 1]) - 1, max(failures_n[:, 1]) + 1])
plt.plot(failures_n[:, 0], failures_n[:, 1], 'bo-')
plt.xlabel("Number of random calls (n) with k = 6")
plt.ylabel("Number of failed independence tests")


plt.show()