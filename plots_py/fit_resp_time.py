import numpy as np
import matplotlib.pyplot as plt
import scipy.stats as stats

alpha = 0.05

job_resp_times_vector = np.genfromtxt("../simulation_results/job_resp_times.csv", delimiter=";")
plt.axis([0, 25, 0, 0.30])
hist_res = plt.hist(job_resp_times_vector, bins=70, normed=True, alpha=0.5)

# exponential lambda
mean = job_resp_times_vector.mean()

x = np.arange(0, 100, 0.1)
# param = stats.expon.fit(job_resp_times_vector)
# calculate ideal exponential pdf with parameter mean
pdf_fitted = stats.expon.pdf(x, scale=mean)

# KS TEST
np.random.seed(12345)
r = np.random.exponential(scale=mean, size=1000)
res = stats.ks_2samp(job_resp_times_vector, r)
if res[1] < alpha:
    print("null hypothesis is rejected with a p-value=" + str(res[1]) + "\n")
else:
    print("null hypothesis cannot be rejected: the samples are drawn by exponential distribution with p-value="+str(res[1]) + "\n")

plt.plot(x, pdf_fitted, 'r', linewidth=3.0)
plt.xlabel("Response time values")
plt.ylabel("Probability")
plt.title("General Response Time Fitting")

plt.show()
