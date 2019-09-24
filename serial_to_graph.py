import matplotlib.pyplot as plt
import numpy as np

FILE = 'zn1.txt'

f = open(FILE, 'r')

lines = f.read().split('\n')

lines = [x for x in lines if x]

lines_dict = {}

times = []
data = []

sb = False
sc = 0

for elem in lines:
    if '---' in elem and not sb:
        sb = True
        time = elem.split(' -> ')[0] 
        timehms = time.split('.')[0].split(':')
        time_millis = int(time.split('.')[1])
        time_millis += int(timehms[2])*1000
        time_millis += int(timehms[1])*60*1000
        time_millis += int(timehms[0])*60*60*1000
        times.append(time_millis)
        data.append([])
    elif '---' in elem and sb:
        sb = False
    else:
        value = float(elem.split(' -> ')[1].split(': ')[1])
        data[-1].append(value)


minor = times[0]
times_temp = [0.0]
for i in range(1, len(times)):
    passed_time = times[i] - minor
    passed_time /= 1000.0
    times_temp.append(passed_time)


data1 = []
data2 = []
data3 = []

for i in range(0, len(data)):
    data1.append(data[i][0])
    data2.append(data[i][1])
    data3.append(data[i][2])

times = times_temp

t = np.array(times)
y1 = np.array(data1)
y2 = np.array(data2)
y3 = np.array(data3)

plt.figure()
plt.subplot(311)
plt.plot(t, y1, 'k')

plt.subplot(312)
plt.plot(t, y2, 'k', color='green')

plt.subplot(313)
plt.plot(t, y3, 'k', color='red')
plt.show()

#print(data)
