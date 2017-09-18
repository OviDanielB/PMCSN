import numpy as np
import matplotlib.pyplot as plt
import subprocess
import random



def printTable(s, a):
    print("  S          0           1          2           3           4           5           6")
    print("-------------------------------------------------------------------------------------------")
    line1 = "  " + s + "   "
    for i in range(0,7):
        line1 += str(a[i])
        line1 += "    "
        if a[i] == 0.0 :
            line1 += "      "
    print(line1)
    print("-------------------------------------------------------------------------------------------")
    print("  S          7           8          9          10          11          12          13")
    print("-------------------------------------------------------------------------------------------")
    line2 = "  " + s + "   "
    for i in range(7, 14):
        line2 += str(a[i])
        line2 += "    "
        if a[i] == 0.0:
            line2 += "      "
    print(line2)
    print("-------------------------------------------------------------------------------------------")
    print("  S         14           15          16         17           18          19           20")
    print("-------------------------------------------------------------------------------------------")
    line3 = "  " + s + "   "
    for i in range(14,21):
        line3 += str(a[i])
        line3 += "    "
        if a[i] == 0.0 :
            line3 += "      "
    print(line3)
    print("-------------------------------------------------------------------------------------------")



N=20
# varies S from 0 to N to write on file simulation results
def generate_data_from_simulation(n):
    seed = 1234
    for x in range(n + 1):
        subprocess.check_output(["../PMCSN", str(N), str(x), "15", "512", str(seed)])
        seed += random.random() * 100 # seed changes


#generate_data_from_simulation(N)



# 1: t1 cloudlet, 2: t1 cloud, 3: t1 global, 4: t2 cloudlet, 5: t2 cloud, 6: t2 global, 7: t cloudlet global, 8: t cloud global
# 9: tht1 cldlet, 10: tht1 cloud, 11: tht1 global, 12: tht2 cldlet, 13 : tht2 cloud, 14: tht2 global,
# 15: t2 wasted, 16: interrupted

# load data from file
a = np.genfromtxt("../simulation_results/resps_thts.csv", delimiter=";")

s = len(a) - 1

s_int = []
for i in range(s+1):
    s_int.append(int(a[i, 0]))


plt.figure(1)

resp_time_min_cldlet1 = min(a[:, 1]) - min(a[:, 1]) / 4
resp_time_max_cldlet1 = max(a[:, 1]) + max(a[:, 1]) / 4

throughput_min_cldlet1 = min(a[:, 9]) - min(a[:, 9]) / 4
throughput_max_cldlet1 = max(a[:, 9]) + max(a[:, 9]) / 4

plt.subplot(211)
plt.axis([0, s, resp_time_min_cldlet1, resp_time_max_cldlet1])
plt.title("Response time - Class 1 on Cloudlet")
plt.plot(a[:, 1], "bo")
plt.plot(a[:, 1], "k")
plt.ylabel("Time")
plt.xticks(s_int)


plt.figure(2)
plt.subplot(211)
plt.axis([0, s, throughput_min_cldlet1, throughput_max_cldlet1])
plt.title("Throughput - Class 1 on Cloudlet")
plt.plot(a[:, 9], "ro")
plt.plot(a[:, 9], "k")
plt.xlabel("S Threshold")
plt.ylabel("Time")
plt.xticks(s_int)


plt.figure(3)
plt.subplot(211)

resp_time_min_cldlet2 = min(a[:, 4]) - min(a[:, 4]) / 4
resp_time_max_cldlet2 = max(a[:, 4]) + max(a[:, 4]) / 4

throughput_min_cldlet2 = min(a[:, 12]) - min(a[:, 12]) / 4
throughput_max_cldlet2 = max(a[:, 12]) + max(a[:, 12]) / 4


plt.subplot(211)
plt.axis([0, s, resp_time_min_cldlet2, resp_time_max_cldlet2])
plt.title("Response time - Class 2 on Cloudlet")
plt.plot(a[:, 4], "bo")
plt.plot(a[:, 4], "k")
plt.ylabel("Time")
plt.xticks(s_int)

plt.figure(4)
plt.subplot(211)
plt.axis([0, s, throughput_min_cldlet2, throughput_max_cldlet2])
plt.title("Throughput - Class 2 on Cloudlet")
plt.plot(a[:, 12], "ro")
plt.plot(a[:, 12], "k")
plt.xlabel("S Threshold")
plt.ylabel("Time")
plt.xticks(s_int)



plt.figure(5)
plt.subplot(211)

resp_time_min_cloud1 = min(a[:, 2]) - min(a[:, 2]) / 4
resp_time_max_cloud1 = max(a[:, 2]) + max(a[:, 2]) / 4

throughput_min_cloud1 = min(a[:, 10]) - min(a[:, 10]) / 4
throughput_max_cloud1 = max(a[:, 10]) + max(a[:, 10]) / 4

plt.axis([0, s, resp_time_min_cloud1, resp_time_max_cloud1])
plt.title("Response time - Class 1 on Cloud")
plt.plot(a[:, 2], "bo")
plt.plot(a[:, 2], "k")
plt.ylabel("Time")
plt.xticks(s_int)

plt.figure(6)
plt.subplot(211)
plt.axis([0, s, throughput_min_cloud1, throughput_max_cloud1])
plt.title("Throughput - Class 1 on Cloud")
plt.plot(a[:, 10], "ro")
plt.plot(a[:, 10], "k")
plt.xlabel("S Threshold")
plt.ylabel("Time")
plt.xticks(s_int)

plt.figure(7)
plt.subplot(211)

resp_time_min_cloud2 = min(a[:, 5]) - min(a[:, 5]) / 4
resp_time_max_cloud2 = max(a[:, 5]) + max(a[:, 5]) / 4

throughput_min_cloud2 = min(a[:, 13]) - min(a[:, 13]) / 4
throughput_max_cloud2 = max(a[:, 13]) + max(a[:, 13]) / 4

plt.axis([0, s, resp_time_min_cloud2, resp_time_max_cloud2])
plt.title("Response time - Class 2 on Cloud")
plt.plot(a[:, 5], "bo")
plt.plot(a[:, 5], "k")
plt.ylabel("Time")
plt.xticks(s_int)

plt.figure(8)
plt.subplot(211)

plt.axis([0, s, throughput_min_cloud2, throughput_max_cloud2])
plt.title("Throughput - Class 2 on Cloud")
plt.plot(a[:, 13], "ro")
plt.plot(a[:, 13], "k")
plt.xlabel("S Threshold")
plt.ylabel("Time")
plt.xticks(s_int)

plt.figure(9)
plt.subplot(211)

resp_time_min_1 = min(a[:, 3]) - min(a[:, 3]) / 4
resp_time_max_1 = max(a[:, 3]) + max(a[:, 3]) / 4

throughput_min_1 = min(a[:, 11]) - min(a[:, 11]) / 4
throughput_max_1 = max(a[:, 11]) + max(a[:, 11]) / 4

plt.axis([0, s, resp_time_min_1, resp_time_max_1])
plt.title("Response time - Class 1")
plt.plot(a[:, 3], "bo")
plt.plot(a[:, 3], "k")
plt.ylabel("Time")
plt.xticks(s_int)

plt.figure(10)
plt.subplot(211)
plt.axis([0, s, throughput_min_1, throughput_max_1])
plt.title("Throughput - Class 1")
plt.plot(a[:, 11], "ro")
plt.plot(a[:, 11], "k")
plt.xlabel("S Threshold")
plt.ylabel("Time")
plt.xticks(s_int)

plt.figure(11)
plt.subplot(211)

resp_time_min_2 = min(a[:, 6]) - min(a[:, 6]) / 4
resp_time_max_2 = max(a[:, 6]) + max(a[:, 6]) / 4

throughput_min_2 = min(a[:, 14]) - min(a[:, 14]) / 4
throughput_max_2 = max(a[:, 14]) + max(a[:, 14]) / 4

plt.axis([0, s, resp_time_min_2, resp_time_max_2])
plt.title("Response time - Class 2")
plt.plot(a[:, 6], "bo")
plt.plot(a[:, 6], "k")
plt.ylabel("Time")
plt.xticks(s_int)

plt.figure(12)
plt.subplot(211)

plt.axis([0, s, throughput_min_2, throughput_max_2])
plt.title("Throughput - Class 2")
plt.plot(a[:, 14], "ro")
plt.plot(a[:, 14], "k")
plt.xlabel("S Threshold")
plt.ylabel("Time")
plt.xticks(s_int)


plt.figure(13)
plt.subplot(211)

resp_time_min_cloudlet = min(a[:, 7]) - min(a[:, 7]) / 4
resp_time_max_cloudlet = max(a[:, 7]) + max(a[:, 7]) / 4

throughput_min_cloudlet = min((a[:, 9])+(a[:, 12])) - min((a[:, 9])+(a[:, 12])) / 4
throughput_max_cloudlet = max((a[:, 9])+(a[:, 12])) + max((a[:, 9])+(a[:, 12])) / 4

plt.axis([0, s, resp_time_min_cloudlet, resp_time_max_cloudlet])
plt.title("Response time - Cloudlet")
plt.plot(a[:, 7], "bo")
plt.plot(a[:, 7], "k")
plt.ylabel("Time")
plt.xticks(s_int)

# printTable("E[t]", a[:, 7])

plt.figure(14)
plt.subplot(211)

plt.axis([0, s, throughput_min_cloudlet, throughput_max_cloudlet])
plt.title("Throughput - Cloudlet")
plt.plot((a[:, 9])+(a[:, 12]), "ro")
plt.plot((a[:, 9])+(a[:, 12]), "k")
plt.xlabel("S Threshold")
plt.ylabel("Time")
plt.xticks(s_int)

# printTable("Thro", a[:, 9])

plt.figure(15)
plt.subplot(211)

resp_time_min_cloud = min(a[:, 8]) - min(a[:, 8]) / 4
resp_time_max_cloud = max(a[:, 8]) + max(a[:, 8]) / 4

throughput_min_cloud = min((a[:, 10])+(a[:, 13])) - min((a[:, 10])+(a[:, 13])) / 4
throughput_max_cloud = max((a[:, 10])+(a[:, 13])) + max((a[:, 10])+(a[:, 13])) / 4

plt.axis([0, s, resp_time_min_cloud, resp_time_max_cloud])
plt.title("Response time - Cloud")
plt.plot(a[:, 8], "bo")
plt.plot(a[:, 8], "k")
plt.ylabel("Time")
plt.xticks(s_int)

# printTable("E[t]", a[:, 8])


plt.figure(16)
plt.subplot(211)

plt.axis([0, s, throughput_min_cloud, throughput_max_cloud])
plt.title("Throughput - Cloud")
plt.plot((a[:, 10])+(a[:, 13]), "ro")
plt.plot((a[:, 10])+(a[:, 13]), "k")
plt.xlabel("S Threshold")
plt.ylabel("Time")
plt.xticks(s_int)

printTable("Thro", a[:, 10])

plt.figure(17)

resp_time_wasted_min = min(a[:, 15]) - min(a[:, 15]) / 4
resp_time_wasted_max = max(a[:, 15]) + max(a[:, 15]) / 4

interrupt_min = min(a[:, 16]) - min(a[:, 16]) / 4
interrupt_max = max(a[:, 16]) + max(a[:, 16]) / 4

plt.subplot(211)
plt.axis([0, s, resp_time_wasted_min, resp_time_wasted_max])
plt.title("Wasted service time - Cloudlet")
plt.plot(a[:, 15], "bo")
plt.plot(a[:, 15], "k")
plt.ylabel("Time")
plt.xticks(s_int)

plt.figure(18)
plt.subplot(211)

plt.axis([0, s, interrupt_min, interrupt_max])
plt.title("Interrupts per seconds - Cloudlet")
plt.plot(a[:, 16], "ro")
plt.plot(a[:, 16], "k")
plt.xlabel("S Threshold")
plt.ylabel("Time")
plt.xticks(s_int)

plt.show()