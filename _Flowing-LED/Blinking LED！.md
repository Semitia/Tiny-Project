# Blinking LED！

## LED结构体

```c
typedef struct __LED_t{
	bool direction;//1:+  ;  0:-
	int16_t luminance;//亮度
	u8 change_speed;//变化速度
	u16 max_lum;//最大亮度
	u16 min_lum;//最小亮度
	void (*p_set_luminance)(int16_t luminance);//控制函数
}LED_t;
```

​	可以通过配置成员很方便的搭配出各种样式。但是在设计初始数据的时候也要注意，范围要能被速度**整除**，不然的话误差会不断积累扩大，最后就失去规律了。不过也不能说失去规律，因为它仍然是有**周期性**的，或许可以利用这一点设计出更复杂的流程。

## 整体流程

- **自动状态**：定时在**慢速模式**、**快速模式**、**熄灭**状态间循环切换
- **按键模式**：通过两个按键，向前一种或者后一种状态切换。状态可继承。（其实这个想法来自于**PLC**工训😂，当时很痛苦，用C语言就愉快多了）
- 通过第三个按键切换这两种模式。

## 8种GPIO

​	网络上有很多介绍GPIO的文章，我就简单讲讲自己的理解吧（可能不是太准确）。

​	上拉输入和下拉输入会分别闭合上拉开关或下拉开关。在外部没有输入的情况下，就分别是高电平和低电平，比较稳定。浮空输入呢就是两个开关都不闭合，会根据外部输入的高低来变高变低。没有输入时状态不确定。模拟输入是直接输入模拟量。开漏输出是输出电平控制场效应管实现输出，需要提供外部电源。推挽输出是利用一个NPN型和一个PNP型三极管，低损耗地输出和变换电平。复用开漏和推挽输出的是外设的数据，而不复用的话输出的是数据寄存器里的数据。

## 小小感想	

​	其实在这之前也没有尝试过使用面向对象的思想去点亮LED灯哈哈。但是在阅读了群里发的代码规范后，也算是颇有感触。

​	反思一下，有时候我也觉得自己的代码写得有点不够**“规范”**，如果隔了一长段时间后，可能有的代码自己也要多看一会才能回想起来。虽然说我也经常写注释，或者代码日志，但是感觉还差了点意思。

​	同样是点流水灯，现在的我与半年前的我相比能有什么样的进步呢？所以借着这次机会，我也想尝试用**面向对象**的思想（应该能算是吧😄），做一个稍微不一样的流水灯。

​	事实上，就从写代码和调试的感受来说，确实感觉不错。之前的话，可能都用的是偏向**模拟**的思路，一段代码只能对应一种样式。而且对象多了之后，或者稍微复杂了一些呢，就容易把自己搞糊涂。事后再看过来也不容易理解。但是**封装**起来之后呢，我可以稍微改动一些参数，接可以设计出不同的花样，也方便理解。

## 遗留问题与后续优化方向

### 代码精简

​	还有很多地方是可以继续精简整合的，比如说这里：

![image-20221009111703051](C:\Users\Semitia\AppData\Roaming\Typora\typora-user-images\image-20221009111703051.png)

### 中断问题

​	期间遇到了一个比较奇怪的问题，同样配置的四个按键，有的反应滞后，有的不灵敏，有的正常等等。串口影响，中断优先级，或者消抖等等都没能解决。

### “无级变速”

​	在自动控制模式，状态的改变是“跳跃式”的，我觉得不够优雅。如果直接改速度的话，会造成在边缘处累计偏差的情况。所以我想利用**相位**，保持灯之间的相位差就可以。