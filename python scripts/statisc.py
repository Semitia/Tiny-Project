import os
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
from mpl_toolkits.mplot3d import Axes3D
import matplotlib
# print(matplotlib.__version__)

h=0.04494988870551895

# 读取./6d_force/food文件夹下food.npy
data_list = np.load("./6d_force/food/food.npy")
# 创建6个空列表来存储x,y,z方向的力和力矩
force_x = []
force_y = []
force_z = []
torque_x = []
torque_y = []
torque_z = []
#给6个列表赋值
for data in data_list:
    force_x.append(data[0])
    force_y.append(data[1])
    force_z.append(data[2])
    torque_x.append(data[3])
    torque_y.append(data[4])
    torque_z.append(data[5])
# 绘制各个力和力矩的分布直方图
# plt.figure()
# plt.subplot(2,3,1)#画布分为2行3列6个子图，这是第1个子图
# plt.hist(force_x, bins=100)#直方图，bins表示直方图的柱数，也就是100个子区间
# plt.title("force_x")
# plt.subplot(2,3,2)
# plt.hist(force_y, bins=100)
# plt.title("force_y")
# plt.subplot(2,3,3)
# plt.hist(force_z, bins=100)
# plt.title("force_z")
# plt.subplot(2,3,4)
# plt.hist(torque_x, bins=100)
# plt.title("torque_x")
# plt.subplot(2,3,5)
# plt.hist(torque_y, bins=100)
# plt.title("torque_y")
# plt.subplot(2,3,6)
# plt.hist(torque_z, bins=100)
# plt.title("torque_z")
# plt.show()

# 绘制六边形图
# plt.hexbin(force_x, force_y, gridsize=20)
# plt.xlabel('force_x')
# plt.ylabel('force_y')
# plt.colorbar()
# plt.show()

# # 假设你的force_x和force_y是列表
# force_x = pd.Series (force_x)
# force_y = pd.Series (force_y)
# force_z = pd.Series (force_z)
# # 绘制单变量核密度图
# force_z.plot.kde (color='red')
# plt.show ()
# # 绘制双变量核密度图
# df = pd.DataFrame ({'force_x': force_x, 'force_y': force_y})
# df.plot.kde ()
# plt.show ()

# pd.Series (force_x).plot.kde (color='red')

# 散点图
#force_x 和 force_y
# plt.scatter (force_x, force_y, s=0.1, color='blue')
# plt.xlabel ('force_x')
# plt.ylabel ('force_y')

# torque_x 和 torque_y
# plt.scatter (torque_x, torque_y, s=0.1, color='blue')
# plt.xlabel ('torque_x')
# plt.ylabel ('torque_y')

# force_y and torque_x
# plt.scatter (force_y, torque_x, s=0.1, color='blue')
# plt.xlabel ('force_y')
# plt.ylabel ('torque_x')

# force_x and torque_y
# plt.scatter (force_x, torque_y, s=0.1, color='blue')
# plt.xlabel ('force_x')
# plt.ylabel ('torque_y')

# plt.show ()

# 绘制三维曲面图
# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')
# ax.plot_surface(force_x, force_y, force_z)
# ax.set_xlabel('force_x')
# ax.set_ylabel('force_y')
# ax.set_zlabel('force_z')
# plt.show()

# 绘制三维散点图
# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')

# # force_x and force_y and force_z
# ax.scatter3D(force_x, force_y, force_z, s=0.1,color='red')
# ax.set_xlabel('force_x')
# ax.set_ylabel('force_y')
# ax.set_zlabel('force_z')

# # torque_x and torque_y and torque_z
# # ax.scatter3D(torque_x, torque_y, torque_z,s=0.1,color='red')
# # ax.set_xlabel('torque_x')
# # ax.set_ylabel('torque_y')
# # ax.set_zlabel('torque_z')

# plt.show()

#位置分布
x = []
y = []

center_list = np.load("./image/food/food.npy")
for center in center_list:
    x.append(center[0])
    y.append(center[1])

# x[i] = torque_y[i]/force_z[i]
# y[i] = torque_x[i]/force_z[i]
#给x,y列表赋值
# Mx = -Fy*h + Fz*y
# My =  Fx*h - Fz*x
# x = (Fx*h - My)/Fz
# y = (Mx + Fy*h)/Fz
# len = len(force_x)
# #for遍历列表
# for i in range(len):
#     #每行打印一个列表中的六个值
#     # print(force_x[i], force_y[i], force_z[i], torque_x[i], torque_y[i], torque_z[i])

#     x.append((force_x[i]*h-torque_y[i])/force_z[i])
#     y.append((torque_x[i]+force_y[i]*h)/force_z[i])

# sns.distplot(x, kde=True)
# # plt.xlim(-0.02, 0.02)
# plt.xlabel('x')
# plt.show()
# sns.distplot(y, kde=True)
# # plt.xlim(-0.02, 0.02)
# plt.xlabel('y')
# plt.show()

My_fx = []
Mx_fy = []
scale_x = 400
scale_y = 400
bias_x = 1
bias_y = 0.5
for i in range(len(x)):
    x[i] = x[i]/scale_x-bias_x
    y[i] = y[i]/scale_y-bias_y
    Mx_fy.append(torque_x[i] - force_z[i]*y[i]) 
    My_fx.append(torque_y[i] + force_z[i]*x[i]) 
sns.jointplot(x=Mx_fy, y=force_y, kind='kde')
plt.show()
sns.set(style="whitegrid")
tips = sns.load_dataset("tips")
g = sns.jointplot(x=force_x, y=force_y, kind='kde', color='purple')
g.plot_joint(sns.kdeplot,fill=True)
plt.show()


# 绘制分布直方图，范围为-0.02到0.02，分为100个子区间
# plt.hist(x, bins=100,range=(-0.02,0.02))
# plt.title("x")
# plt.show()
# plt.hist(y, bins=100)
# plt.title("y")
# plt.show()
# 绘制两者的散点图,范围-0.02到0.02
# plt.scatter (force_z, x, s=0.1, color='blue')
# plt.show()




## seaborn

# 绘制六个单变量核密度图
# sns.distplot(force_x, kde=True)
# plt.xlabel('force_x')
# plt.show()
# sns.distplot(force_y, kde=True)
# plt.xlabel('force_y')
# plt.show()
# sns.distplot(force_z, kde=True)
# plt.xlabel('force_z')
# plt.show()
# sns.distplot(torque_x, kde=True)
# plt.xlabel('torque_x')
# plt.show()
# sns.distplot(torque_y, kde=True)
# plt.xlabel('torque_y')
# plt.show()
# sns.distplot(torque_z, kde=True)
# plt.xlabel('torque_z')
# plt.show()

# 绘制六个双变量核密度图
# sns.jointplot(x=force_x, y=force_y, kind='kde')
# plt.show()
# sns.set(style="whitegrid")
# tips = sns.load_dataset("tips")
# g = sns.jointplot(x=force_x, y=force_y, kind='kde', color='purple')
# g.plot_joint(sns.kdeplot,fill=True)
# plt.show()

