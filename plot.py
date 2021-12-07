#!/usr/bin/python3

# Copyright (c) 2021 Giannis Gonidelis

import matplotlib.pyplot as plt
import numpy as np
import csv 

# ids = []
# values = []
results_list = []
x = 0

with open('result.csv','r') as csvfile:    
    lines = csv.reader(csvfile, delimiter=';')
    
    for row in lines:   
        if(x % 2 == 0):
            ids = row
        else:
            values = row
            d = dict(zip(ids, values))
            results_list.append(d)
            # print(d)
        x += 1


def key_func(k):
    return k['name']

results_list = sorted(results_list, key=key_func)
middle = len(results_list)//2
fork_join = results_list[:middle]
pipe_range = results_list[middle:]

problem_size = (2**exp for exp in range(15, 30))
elements = [n for n in problem_size]
    
fig, ax = plt.subplots()
ax.set_xscale('log', base=2)

plt.plot(elements, [float(li["elapsed"]) for li in fork_join], linestyle = 'dashed', 
    marker = 'o', label="transform(); foreach()")
plt.plot(elements, [float(li["elapsed"]) for li in pipe_range], linestyle = 'dashed', 
    marker = 'o', label="foreach(transform view)")

plt.xlabel("Number of Elements")
plt.ylabel("time[s]")
plt.grid()
ax.legend()

plt.show()

# x = x[::-1]
# x = [int(i) for i in x]
# y = y[::-1]
# plt.plot(x, y, linestyle = 'dashed',
#         marker = 'o', label=i)