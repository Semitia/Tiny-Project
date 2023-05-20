import os
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
from mpl_toolkits.mplot3d import Axes3D

#读取./cliber/wrench下的所有.npy文件
data_list = []
for root, dirs, files in os.walk("./cliber/wrench"):
    for file in files:
        if os.path.splitext(file)[1] == '.npy':
            data_list.append(np.load(os.path.join(root, file)))
h=[]
for data in data_list:
    hi = data[3]/data[1]
    h.append(hi)
    print(hi)

#取h平均值
h_mean = np.mean(h)
#打印字符串"平均值为："
print("平均值为：")
print(h_mean)