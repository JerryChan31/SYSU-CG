# SYSU-CG
中山大学2018年计算机图形学课程作业 by JerryChan

Progress: 

hw1 finished. 

hw2 finished. 

hw3 finished. 

hw4 finished. 

hw5 finished. 

hw6 finished. 

hw7 finished. 

## HW1 基础图形

1. 使用OpenGL(3.3及以上)+GLFW或freeglut画一个简单的三角形。
2. 对三角形的三个顶点分别改为红绿蓝。
3. 给上述工作添加一个GUI，里面有一个菜单栏，使得可以选择并改变三角形的颜色。
4. 绘制其他的图元，除了三角形，还有点、线等。
5. 使用EBO(Element Buffer Object)绘制多个三角形。

![]()

## HW2 Bresenham算法

1. 使用Bresenham算法(只使用integer arithmetic)画一个三角形边框：input为三个2D点；output三条直
线（要求图元只能用 GL_POINTS ，不能使用其他，比如 GL_LINES 等）。
2. 使用Bresenham算法(只使用integer arithmetic)画一个圆：input为一个2D点(圆心)、一个integer半
径；output为一个圆。
3. 在GUI在添加菜单栏，可以选择是线还是圆，以及能调整圆的大小(圆心固定即可)。
4. 使用三角形光栅转换算法，用和背景不同的颜色，填充你的三角形。

![]()

## HW3 

1. 画一个立方体(cube)：边长为0.4， 中心位置为(0, 0, 0)。分别启动和关闭深度测试 `glEnable(GL_DEPTH_TEST)` 、 `glDisable(GL_DEPTH_TEST)`。
2. 平移(Translation)：使画好的cube沿着水平或垂直方向来回移动。
3. 旋转(Rotation)：使画好的cube沿着YoZ平面的y=z轴持续旋转。
4. 放缩(Scaling)：使画好的cube持续放大缩小。
5. 在GUI里添加菜单栏，可以选择各种变换。

![]()

## HW4 

1. 投影(Projection):
把上次作业绘制的cube放置在(-1.5, 0.5, -1.5)位置，要求6个面颜色不一致
**正交投影(orthographic projection)**：实现正交投影，使用多组(left, right, bottom, top, near, far)参
数，比较结果差异。
**透视投影(perspective projection)**：实现透视投影，使用多组参数，比较结果差异。
2. 视角变换(View Changing):
把cube放置在(0, 0, 0)处，做透视投影，使摄像机围绕cube旋转，并且时刻看着cube中心
3. 在GUI里添加菜单栏，可以选择各种功能。 Hint: 使摄像机一直处于一个圆的位置，可以参考以下公
式：
    camPosX=sin(clock()/1000.0)*Radius;
    camPosZ=cos(clock()/1000.0)*Radius;

原理很容易理解，由于圆的公式 a^2+b^2=1 ，以及有 sin(x)^2+cos(x)^2=1 ，所以能保证摄像机在XoZ平面的一个圆上。

![]()

## HW5

1. 实现Phong光照模型：
场景中绘制一个cube，自己写shader实现两种shading: Phong Shading 和 Gouraud Shading，并解释两种shading的
实现原理，合理设置视点、光照位置、光照颜色等参数，使光照效果明显显示。
2. 使用GUI，使参数可调节，效果实时更改：GUI里可以切换两种shading，使用如进度条这样的控件，使ambient因子、diffuse因子、specular因子、反光度等参数可调节，光照效果实时更改。

![]()

## HW6

1. 实现Shadowing Mapping:
要求场景中至少有一个object和一块平面(用于显示shadow)，光源的投影方式任选其一即可。
2. 修改GUI。
1. 实现光源在正交/透视两种投影下的Shadowing Mapping。
2. 优化Shadowing Mapping。

![]()

## HW7

1. 用户能在工具屏幕上画4个点（使用鼠标点击），然后工具会根据这4个点拟合出一条Bezier
Curve（按照画点的顺序）
2. 用户画完第一条Bezier Curve之后，可以调整4个点的位置。工具会根据调整位置后的点实时更新曲
线的样子。
3. 在GUI里添加菜单栏，用户可以选择Bezier Curve的颜色。
4. 用户画点时，可以把画出的某个点消除。

![]()