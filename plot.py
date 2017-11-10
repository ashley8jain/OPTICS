import matplotlib.pyplot as plt
import numpy as np

reach_dist=[]

with open("reach_dist.tsv") as f:
    for line in f:
    	reach_dist.append(float(line))


new_reach = []

for num in reach_dist:
	if num>1e+100:
		new_reach.append(0)
	else:
		new_reach.append(num)	

y_pos = np.arange(len(new_reach))
# print y_pos

plt.bar(y_pos,new_reach)
plt.show()