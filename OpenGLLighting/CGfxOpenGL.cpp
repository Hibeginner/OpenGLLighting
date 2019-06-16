#include "stdafx.h"

/*
(x, y, z)物体的顶点坐标经过
模型矩阵，平移缩放等，变成世界坐标
视图矩阵，根据摄像机的位置，变成眼睛坐标、相机坐标
投影矩阵，相机的视椎体做投影变换，远近裁剪面，梯形变成-1到1的规范的标准立方体，物体坐标就在立方体内
视口变换，变成窗口坐标
*/

//禁止把单精度变双精度
#pragma warning(disable:4305)

void ShowFloat(float f) {
	TCHAR a[32];
	swprintf(a, 32, _T("%f"), f);

	MessageBox(NULL, a, _T("Test"), MB_OK);
}

CGfxOpenGL::CGfxOpenGL() {

}

CGfxOpenGL::~CGfxOpenGL() {
	
}

bool CGfxOpenGL::Init() {
	theRobot = new Robot();
	glClearColor(0.0, 0.0, 0.0, 0.0);

	rotateAngle = 0;

	glEnable(GL_DEPTH_TEST);//深度测试，近的挡住远的
	glDepthFunc(GL_LEQUAL);//深度测试小于等于算法。只有后者距离小于等于前者，就显示后者。距离摄像机的距离越小，越优先显示

	glEnable(GL_LIGHTING);//启用光照。还需要激活灯光
	GLfloat light_ambient[4] = { 0.3, 0.3, 0.3, 1.0};//环境光 0.3弱
	GLfloat light_diffuse[4] = { 1.0, 1.0, 1.0, 1.0 };//漫反射光
	GLfloat light_specular[4] = { 1.0, 1.0, 1.0, 1.0 };//镜面反射光
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);//定义灯光的属性
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	GLfloat lightPos[4] = { 0,100,100,1 };//默认0,0,1的位置。第四个参数为1，为定点光。0平行光
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);//激活灯光

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//GLfloat pointSize;
	//glGetFloatv(GL_POINT_SIZE, &pointSize);//点的大小，单位为像素
	//ShowFloat(pointSize);

	//if (!glIsEnabled(GL_POINT_SMOOTH)) {
	//	glEnable(GL_POINT_SMOOTH);//对 GL_POINST 点 做平滑处理，显示成圆形。否则会是方形
	//}

	//GLfloat sizes[2];
	//glGetFloatv(GL_POINT_SIZE_RANGE, sizes);//启用点的平滑处理后，OpenGL对点的大小有要求，小于或大于这个边界的点，处理就有问题
	//GLfloat minPointSize = sizes[0];
	//GLfloat maxPointSize = sizes[1];
	//ShowFloat(minPointSize);
	//ShowFloat(maxPointSize);

	//GLfloat granularity;//颗粒度
	//glGetFloatv(GL_POINT_SIZE_GRANULARITY, &granularity);
	//ShowFloat(granularity);//0.125(应该是像素)这个间隔的点 画出来的效果最好。启用平滑处理后，画点的时候，点的大小会被OpenGL做成颗粒度的倍数


	//  1 / (a + b*d + c*d²)衰减系数   d为距离  默认a=1 b=0 c=0
	//GLfloat att[3] = { 0.0, 0.0, 0.1 };
	//glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, att);//改变点的距离衰减
														   //glPointParameteri(GL_POINT_SIZE_MAX, 2f);//衰减时，可以设置最大点大小 最小点大小 GL_POINT_FADE_THRESHOLD小的看不清 大的能看清的渐变

														   //画线时也有线的粗细，斜线会出现锯齿。GL_LINE_SMOOTH可以设置抗锯齿
														   //抗锯齿也有区间，也有颗粒度

	return true;
}

bool CGfxOpenGL::Shutdown() {
	delete theRobot;
	return true;
}

void CGfxOpenGL::SetupProjection(int width, int height) {


	if (height == 0) {
		height = 1;
	}

	glViewport(0, 0, width, height);//设置OpenGL视口大小。看得见的区域。OpenGL可以很大。矩阵变换中的最后一步，视口变换，视口矩阵，物体本地坐标变成窗口坐标

	glMatrixMode(GL_PROJECTION);//选择投影矩阵
	glLoadIdentity();//投影矩阵设为单位矩阵。（即把相机放回原点，朝向Z轴负方向。这一句解释可能不对）

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);//透视，近大远小。近裁剪面，远裁剪面。投影变换

	glMatrixMode(GL_MODELVIEW);//模型视图矩阵。OpenGL将模型矩阵和视图矩阵合二为一。
	glLoadIdentity();

	m_windowWidth = width;
	m_windowHeight = height;
}

void CGfxOpenGL::ResizeScene(int width, int height) {
	if (height == 0) {
		height = 1;
	}

	glViewport(0, 0, width, height);//X，Y————以像素为单位，指定了视口的左下角（在第一象限内，以（0，0）为原点的）位置。width，height————表示这个视口矩形的宽度和高度，根据窗口的实时变化重绘窗口。
	
	m_windowWidth = width;
	m_windowHeight = height;
	UpdateProjection(false);
}

void CGfxOpenGL::UpdateProjection(bool toggle) {

	static bool useProjective = true;
	if (toggle) {
		useProjective = !useProjective;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (useProjective) {
		//glFrustum(-1.0, 1.0, -1.0, 1.0, 1, 1000.0);//用来设置视椎体。透视
		gluPerspective(52.0f, (GLfloat)m_windowWidth / (GLfloat)m_windowHeight, 1.0f, 1000.0f);//透视，近大远小。近裁剪面，远裁剪面。投影变换
	}
	else
	{
		glOrtho(-1.0, 1.0, -1.0, 1.0, 1, 1000.0);//正交
	}
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CGfxOpenGL::DrawCube(float xPos, float yPos, float zPos)
{
	glPushMatrix();//将前面的矩阵压入堆栈，下面的矩阵变换不会影响之前的矩阵（之前在Render方法里有模型视图矩阵的旋转平移）。下面的矩阵也是在世界坐标系里的。好像没有任何变化
	
	glTranslatef(xPos, yPos, zPos);
	
	glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);//法线
		glVertex3f(0.0f, 0.0f, 0.0f);//top 逆时针画四方形
		glVertex3f(0.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, 0.0f, 0.0f);

		glNormal3f(0.0f, 0.0f, 1.0f);//法线
		glVertex3f(0.0f, 0.0f, 0.0f);//front
		glVertex3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glVertex3f(0.0f, -1.0f, 0.0f);

		glNormal3f(1.0f, 0.0f, 0.0f);//法线
		glVertex3f(0.0f, 0.0f, 0.0f);//right
		glVertex3f(0.0f, -1.0f, 0.0f);
		glVertex3f(0.0f, -1.0f, -1.0f);
		glVertex3f(0.0f, 0.0f, -1.0f);

		glNormal3f(-1.0f, 0.0f, 0.0f);//法线
		glVertex3f(-1.0f, 0.0f, 0.0f);//left
		glVertex3f(-1.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);

		glNormal3f(0.0f, -1.0f, 0.0f);//法线
		glVertex3f(0.0f, -1.0f, 0.0f);//bottom
		glVertex3f(0.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);

		glNormal3f(0.0f, 0.0f, -1.0f);//法线
		glVertex3f(0.0f, 0.0f, -1.0f);//back
		glVertex3f(-1.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(0.0f, -1.0f, -1.0f);
	glEnd();

	glPopMatrix();
}

void CGfxOpenGL::Prepare(float dt) {
	rotateAngle += 45.0f * dt;
	if (rotateAngle >= 360) {
		rotateAngle = 0;
	}

	theRobot->Prepare(dt);
}

void CGfxOpenGL::Render() {
	//3D绘图
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空背景。变黑
	glLoadIdentity();

	gluLookAt(2.0f, 2.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	/*
	自发光（点光源） emissive
                 emissive = K
                            K为材质的放射系数（材质的颜色）
环境光           ambient
                 ambient = K * globalAmbient
                           K为材质的环境光系数（也是个颜色）
                           globalAmbient为全局环境光颜色
漫反射光         diffuse
                 diffuse = K * lightColor * max(N * L, 0)
                           K为材质漫反射系数（也是个颜色）
                           lightColor为漫反射光颜色
                           N为规范化的表面法向量
                           L为规范化的指向光源的向量
镜面反射光       specular
                 光泽度shiniess：镜面反射光，物体越光泽，反射越强。类似聚光灯照在金属球上的效果。
                 specular = K * lightColor * facing * (max(N * H, 0))^shiniess
                            K为材质的镜面反射系数（也是个颜色）
                            lightColor为镜面反射光颜色
                            N为规范化的表面法向量
                            L为规范化的指向光源的向量
                            V为规范化的指向视点的向量
                            H为规范化的L和V的中间向量
                            facing为1或0，如果N * L大于0，facing=1
                            shiniess为光泽度指数

基本光照模型
surfaceColor = emissive + ambient + diffuse + specular
	*/

	GLfloat material_yellow[4] = {0.7, 0.6, 0.1, 1};//rgba
	GLfloat material_white[4] = { 1, 1, 1, 1 };//rgba
	GLfloat material_ambient[4] = { 0.5, 0.5, 0.5, 1 };//rgba
	GLfloat material_black[4] = { 0.0, 0.0, 0.0, 1 };//rgba
	GLfloat material_red[4] = { 0.8, 0.0, 0.0, 1 };//rgba
	glMaterialf(GL_FRONT, GL_EMISSION, 0);//物体的正面的“自发光”设为0，不发光。
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);//material_ambient材质的环境光反射系数 将会乘以 光源的环境光 0.3 *0.5 = 0.15
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_yellow);//材质的漫反射光系数。漫反射光的计算方法比环境光稍复杂。也是乘法。网上有公式
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_black);//镜面反射光系数。系数为0，就是没有
	glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);//光泽度系数。计算镜面反射光时用到。
	//最终。自发光+环境光+漫反射光+镜面反射光=我们看到的颜色
	glColor3f(1.0f, 0.0f, 0.0f);//启用光照后，glColor就不起效了
	DrawCube(0.0, 0.0, 0.0);

	//glColor3f(7.0f, 1.0f, 0.3f);

	//float pointSize = 0.5f;
	//for (float point = -4.0f; point < 5.0f; point += 0.5f) {
	//	glPointSize(pointSize);

	//	glBegin(GL_POINTS);
	//	    //glVertex3f(0.0f, 0.0f, 0.0f);
	//		//glVertex2i((GLint)point, 0);
	//		GLfloat p[3] = {point, 0.0f, 0.0f};
	//		glVertex3fv(p);
	//	glEnd();

	//	pointSize += 1.0f;
	//}


	/*glBegin(GL_TRIANGLES);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glEnd();*/


	/*glPointSize(9.5f);
	glBegin(GL_POINTS);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f( 1.0f, 0.0f, -2.0f);
	glEnd();*/

	//glEnable(GL_LINE_STIPPLE);//画虚线

	//glLineStipple(2, 0xAAAA);//1010 1010 1010 1010  虚线像素是否显示 和这个序列是相反的。第一个参数2，是每一位重复的次数 11001100 11001100 ..
	//						 //目前表示 2个像素画 2个像素不画

	//glLineWidth(6.55);
	//glBegin(GL_LINES);
	//glVertex3f(-5.0f, 0.0f, 0.0f);
	//glVertex3f(5.0f, 0.0f, -2.0f);
	//glEnd();

	//glPolygonMode(GL_FRONT, GL_LINE);//正面、线框的样式。默认GL_FRONT_AND_BACK,GL_FILL
	//glCullFace(GL_BACK);//指定要剔除的面为背面。背面不画
	//glEnable(GL_CULL_FACE);//启动“剔除面”
	//glFrontFace(GL_CW);//设置正面，为顺时针。默认是GL_CCW与三角形画的顺序有关
	//glBegin(GL_TRIANGLES);//GL_TRIANGLE_STRIP三角形带（逆时针画三角形） GL_TRIANGLE_FAN（扇形）起点公用
	//glEdgeFlag(GL_TRUE);//控制边显不显示
	//glVertex3f(0.0, 5.0, 0.0);
	//glVertex3f(0.0, 0.0, 0.0);
	//glVertex3f(3.0, 5.0, 0.0);
	//glEnd();

	//GL_QUADS四边形
	//GL_QUAD_STRIP四边形带 有顺序的  上下上下
	//GL_POLYGON逆时针方向连续多个定点

	/*glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0, 5.0, 0.0);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(3.0, 5.0, 0.0);
	glEnd();*/

	//glShadeModel(GL_FLAT);//颜色的单调模式，画顶点时 只能用一种颜色。直线、三角形、四边形最后一个点的颜色，线段使用的分线段中的第二个颜色。多边形使用的第一个顶点颜色。GL_SMOOTH平滑，差值渐变


	//glPushMatrix();//把模型视图矩阵压栈。会把当前状态压入第二层，不过此时栈顶的矩阵也与第二层的相同
	//	glLoadIdentity();
	//	glTranslatef(0.0f, 0.0f, -30.0f);
	//	glRotated(rotateAngle, 0.0f, 1.0f, 0.0f);
	//	theRobot->DrawRobot(0.0, 0.0, 0.0);

	//glPopMatrix();//当经过一系列的变换后，栈顶矩阵被修改，此时调用glPopMatrix()时，栈顶矩阵被弹出，且又会恢复为原来的状态。
}