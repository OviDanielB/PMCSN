import numpy as np
import matplotlib.pyplot as plt
import scipy.stats as stats

alpha = 0.05

job_resp_times_vector = np.genfromtxt("../simulation_results/job_resp_times.csv", delimiter=";")
plt.axis([0, 25, 0, 0.30])
res=plt.hist(job_resp_times_vector, bins=70, normed=True, alpha=0.5)
values = res[1]
print(values)

# exponential lambda
mean = job_resp_times_vector.mean()

x = np.arange(0, 25, 0.5)
#param = stats.expon.fit(job_resp_times_vector)
# calculate ideal exponential pdf with parameter mean
pdf_fitted = stats.expon.pdf(x, scale=mean)

#KS TEST
res = stats.kstest(job_resp_times_vector, lambda x: stats.expon.cdf(x, scale=mean))
if(res[1] < alpha):
    print("null hypothesis is rejected")
else:
    print("null hypothesis cannot be rejected: the samples are drawn by exponential distribution")

plt.plot(x, pdf_fitted, 'r', linewidth=3.0)
plt.xlabel("Response time values")
plt.ylabel("Probability")
plt.title("General Response Time Fitting")

plt.show()
