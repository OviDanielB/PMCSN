import numpy as np
import matplotlib.pyplot as plt
import scipy.stats as stats


job_resp_times_vector = np.genfromtxt("../simulation_results/job_resp_times.csv", delimiter=";")
plt.axis([0, 25, 0, 0.30])
plt.hist(job_resp_times_vector, bins=70, normed=True, alpha=0.5)

# exponential lambda
mean = job_resp_times_vector.mean()

x = np.arange(0, 25, 0.5)
#param = stats.expon.fit(job_resp_times_vector)
# calculate ideal exponential pdf with parameter mean
pdf_fitted = stats.expon.pdf(x, scale=mean)

plt.plot(x, pdf_fitted, 'r', linewidth=3.0)
plt.xlabel("Response time values")
plt.ylabel("Probability")
plt.title("General Response Time Fitting")

plt.show()


