"""
RLS算法的实现
"""
import numpy as np
import matplotlib.pyplot as plt


class RLS:
    def __init__(self, dim, theta, p, lambda_):
        self.lambda_ = lambda_
        self.dim = dim
        self.theta = np.array(theta, dtype=np.float64).reshape((dim, 1))
        self.p = np.array(p, dtype=np.float64).reshape((dim, dim))
        self.theta_history = []  # 用于保存theta值的列表

    def update(self, x_in, y):
        x = np.array(x_in).reshape((self.dim, 1))
        e = y - np.dot(self.theta.T, x)[0][0]

        up = np.dot(self.p, x)
        down = np.dot(np.dot(x.T, self.p), x)[0][0] + self.lambda_
        k = up / down

        self.theta += k * e
        self.p = (self.p - np.dot(k, np.dot(x.T, self.p))) / self.lambda_
        # 在每次更新后保存theta值
        self.theta_history.append(self.theta.copy())

    def compute(self, x_in):
        x = np.array(x_in).reshape((self.dim, 1))
        return np.dot(self.theta.T, x)[0][0]


def compute_with_theta(x_in, theta):
    x = np.array(x_in).reshape((3, 1))
    return np.dot(theta.T, x)


def func0(x):
    return 1 * x * x + 2 * x + 300


def func1(x):
    return -2 * x * x + 5 * x + 600


def func2(x):
    return 5 * x * x + 1 * x + 100


rls = RLS(3, [0, 0, 0], [1, 0, 0, 0, 1, 0, 0, 0, 1], 0.9)
y_list0 = []
y_list1 = []
y_list2 = []
y_real_list0 = []
y_real_list1 = []
y_real_list2 = []
y_hat_list0 = []
y_hat_list1 = []
y_hat_list2 = []
theta0 = [0, 0, 0]
theta1 = [0, 0, 0]
theta2 = [0, 0, 0]
DATA_NUM = 100
BIAS_SCALE = 2000
for i in range(DATA_NUM):
    x_gen = [i * i, i, 1]
    y_real = func0(i)
    y_gen = y_real + (np.random.rand() - 0.5) * BIAS_SCALE
    # 将数据保存起来
    y_list0.append(y_gen)
    y_real_list0.append(y_real)
    rls.update(x_gen, y_gen)

print("Estimated parameters:", rls.theta.ravel())
for i in range(DATA_NUM):
    x_gen = [i * i, i, 1]
    y_hat_list0.append(rls.compute(x_gen))


# 绘制图像
plt.plot(y_list0, label='y')
plt.plot(y_real_list0, label='y_real')
plt.plot(y_hat_list0, label='y_hat0')

for i in range(DATA_NUM):
    x_gen = [i * i, i, 1]
    y_real = func1(i)
    y_gen = y_real + (np.random.rand() - 0.5) * BIAS_SCALE
    # 将数据保存起来
    y_list1.append(y_gen)

    rls.update(x_gen, y_gen)

for i in range(DATA_NUM):
    x_gen = [i * i, i, 1]
    y_hat_list1.append(rls.compute(x_gen))

plt.plot(y_list1, label='y')
plt.plot(y_real_list1, label='y_real')
plt.plot(y_hat_list1, label='y_hat1')

for i in range(DATA_NUM):
    x_gen = [i * i, i, 1]
    y_real = func2(i)
    y_gen = y_real + (np.random.rand() - 0.5) * BIAS_SCALE
    # 将数据保存起来
    y_list2.append(y_gen)
    rls.update(x_gen, y_gen)

for i in range(DATA_NUM):
    x_gen = [i * i, i, 1]
    y_hat_list2.append(rls.compute(x_gen))

plt.plot(y_list2, label='y')
plt.plot(y_real_list2, label='y_real')
plt.plot(y_hat_list2, label='y_hat2')

# 在所有迭代完成后，绘制theta值的变化曲线
theta_history = np.array(rls.theta_history)

fig, axs = plt.subplots(3, figsize=(10, 10))
for i in range(3):
    axs[i].plot(theta_history[:, i], label=f'theta{i}')
    axs[i].legend()

plt.legend()
plt.show()

