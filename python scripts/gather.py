#将子文件夹中所有.npy文件中的数据汇总到一个.npy文件中
import os
import cv2
import numpy as np
import matplotlib.pyplot as plt

# 创建一个空列表来存储数据
data_list = []
center_list = []
# 遍历6d_force/food文件夹下的所有子文件夹
for subfolder in os.listdir("6d_force/food"):
    # 拼接子文件夹的完整路径
    subfolder_path = os.path.join("6d_force/food", subfolder)
    # 遍历子文件夹下的前200个.npy文件
    for file in os.listdir(subfolder_path)[:200]:
        # 拼接.npy文件的完整路径
        file_path = os.path.join(subfolder_path, file)
        # 读取.npy文件并转换为numpy数组
        data_array = np.load(file_path)
        # 把数组添加到列表中
        data_list.append(data_array)
np.save("6d_force/food/food.npy", data_list)

# 遍历image/food文件夹下的所有子文件夹
for subfolder in os.listdir("image/food"):
    # 拼接子文件夹的完整路径
    subfolder_path = os.path.join("image/food", subfolder)
    # 遍历子文件夹下的前200个.png文件
    for file in os.listdir(subfolder_path)[:200]:
        # 拼接.png文件的完整路径
        file_path = os.path.join(subfolder_path, file)
        # 读取.png文件
        img=cv2.imread(file_path)
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        # 二值化灰度图像，阈值可以根据实际情况调整
        ret, thresh = cv2.threshold(gray, 150, 255, cv2.THRESH_BINARY_INV)
        # 寻找白色区域的轮廓
        contours, hierarchy = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        filted_contours = []
        centroids = []
        areas = []
        #遍历轮廓列表
        for contour in contours:
            #剔除面积等于0的轮廓
            area = cv2.contourArea(contour)
            if area > 0:
                filted_contours.append(contour)
                M = cv2.moments(contour)
                cx = int(M['m10']/M['m00'])
                cy = int(M['m01']/M['m00'])
                centroids.append([cx, cy])
                areas.append(area)
        #如果areas列表为空，则说明没有找到白色区域，跳过此次循环
        if len(areas) == 0:
            continue

        centroids = np.array(centroids)
        areas = np.array(areas)
        Cx = np.sum(centroids[:,0]*areas)/np.sum(areas)
        Cy = np.sum(centroids[:,1]*areas)/np.sum(areas)
        #转换为整数
        Cx = int(Cx)
        Cy = int(Cy)
        #将形心坐标添加到列表中
        center_list.append([Cx, Cy])
        #每成功处理1000张图片，打印一次提示信息
        if len(center_list) % 1000 == 0:
            print("已处理{}张图片".format(len(center_list)))
            

np.save("image/food/food.npy", center_list)