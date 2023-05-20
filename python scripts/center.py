import os
import cv2
import numpy as np
from matplotlib import pyplot as plt

img_list = []
tresh_list = []
center_list = []
# 读取./cliber/geometry所有子文件夹下所有图片并转换为灰度图像
for subfolder in os.listdir("./cliber/geometry"):
    subfolder_path = os.path.join("./cliber/geometry", subfolder)
    for file in os.listdir(subfolder_path):
        file_path = os.path.join(subfolder_path, file)
        img = cv2.imread(file_path)
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

        # 打印图像名称和形心坐标
        # print(file, Cx, Cy)
        # 在原图上绘制形心点和轮廓
        # cv2.circle(img, (Cx, Cy), 5, (0, 0, 255), -1)
        # cv2.drawContours(img, filted_contours, -1, (0, 255, 0), 2)
        # 将修改后的图像保存到./cliber/mark_result文件夹下
        # cv2.imwrite("./cliber/mark_result/"+file, img)
# 将center_list保存到./cliber/geometry/center.npy文件中
np.save("./cliber/geometry/center.npy", center_list)





# # 显示结果
# cv2.imshow("Result", img)
# cv2.waitKey(0)
# cv2.destroyAllWindows()


# 创建一个窗口，设置标题和大小
#plt.figure("Thresholding", figsize=(10, 5))

# # 在窗口中绘制两个子图，左边是原图，右边是二值化后的图像
# plt.subplot(121), plt.imshow(img, cmap="gray"), plt.title("Original")
# plt.subplot(122), plt.imshow(thresh, cmap="gray"), plt.title("Thresholded")

# # 显示窗口
# plt.show()
