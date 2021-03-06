# STM32_Car
基于STM32的智能(障)小车，实现基本动作驱动控制、手机蓝牙遥控、测距避障等功能。

>  该项目为广州大学硬件方向课程设计项目，时值期末，未免粗糙，仅供自己记录学习。

设备为STM32F103C8，IDE为Keil5.28，Debugger为J-Link。具体设置见后文。

实现功能如下：

+ 基本动作驱动控制模块 `motor`
+ 手机蓝牙遥控模块 `blue`
+ 自动避障（超声波测距避障）*

+ 循迹功能（红外循迹）*
+ 电池管理*

> 暂未实现的功能以`*`标注

## 配置

+ 编译配置

![image-20210623152911694](https://i.loli.net/2021/06/23/XAyBjmYvb1IfOFs.png)

详见`Notes`文件夹下的课程笔记



## 设计步骤

+ 基础操作：GPIO控制引脚变高变低，检测引脚的高地电平、串口发送数据和接收数据
+ 控制电机：控制PWM信号的占空比以控制电机的转速、控制电机方向引脚以控制电机的转动方向
+ 超声波避障*
+ 红外循迹*

## 参考教程

项目参考了课程笔记、博客和视频课程。

课程笔记、原理图示和相关资源见`Notes`文件夹，包含IDE配置、代码说明等

视频可在B站搜：[技新课堂 从零开始制作STM32智能无线避障小车--软硬件教学](https://b23.tv/egKQMd)



To be continued...