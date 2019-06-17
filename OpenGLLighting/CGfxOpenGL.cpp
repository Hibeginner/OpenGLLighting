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

void CGfxOpenGL::DrawPlane(float size, float step) {
	
	glBegin(GL_QUADS);

	glNormal3f(0.0f, -1.0f, 0.0f);
	
	for (float z = 0.0; z < size; z += step) {
		for (float x = 0.0; x < size; x += step) {
			glVertex3f(x, 0.0f, z);
			glVertex3f(x + step, 0.0f, z);
			glVertex3f(x + step, 0.0f, z + step);
			glVertex3f(x, 0.0f, z + step);
		}
	}
	
	glEnd();
}

void CGfxOpenGL::DrawCube(float size, int resolution)
{
	float step = size / resolution;
	glPushMatrix();
		glTranslatef(-size / 2, -size / 2, -size / 2);
		//top
		glPushMatrix();
			glTranslatef(0.0f, size, 0.0f);
			glScalef(1.0f, -1.0f, 1.0f);
			DrawPlane(size, step);
		glPopMatrix();

		//left
		glPushMatrix();
			glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
			glScalef(1.0f, -1.0f, 1.0f);
			DrawPlane(size, step);
		glPopMatrix();

		//right
		glPushMatrix();
			glTranslatef(size, 0.0f, 0.0f);
			glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
			DrawPlane(size, step);
		glPopMatrix();

		//front
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, size);//3. 再平移
			glRotatef(90.0f, -1.0f, 0.0f, 0.0f);//2. 在翻转
			DrawPlane(size, step);//1. 先画出来
		glPopMatrix();

		//back
		glPushMatrix();
			glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
			glScalef(1.0f, -1.0f, 1.0f);
			DrawPlane(size, step);
		glPopMatrix();

		//bottom
		DrawPlane(size, step);
	glPopMatrix();
}

static GLfloat light1_Pos[4] = { -5.0f, 10.0f, 10.0f, 1.0f };
static GLfloat spot_dir[3] = { -1.0f, -2.5f, -10.0f };
bool CGfxOpenGL::Init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);

	rotateAngle = 0;

	glEnable(GL_DEPTH_TEST);//深度测试，近的挡住远的
	glDepthFunc(GL_LEQUAL);//深度测试小于等于算法。只有后者距离小于等于前者，就显示后者。距离摄像机的距离越小，越优先显示

	glEnable(GL_NORMALIZE);//法线标准化处理
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_LIGHTING);//启用光照。还需要激活灯光
	
	GLfloat light_ambient[4] = { 0.3f, 0.3f, 0.3f, 1.0f};//环境光 0.3弱
	GLfloat light_diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };//漫反射光
	GLfloat light_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };//镜面反射光

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);//定义灯光的属性
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	GLfloat lightPos[4] = { 0,100,100,1 };//默认0,0,1的位置。第四个参数为1，为定点光。0平行光
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);//设置衰减公式系数
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01f);//设置衰减公式系数
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);//设置衰减公式系数

	glEnable(GL_LIGHT0);//激活灯光


	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	
	glLightfv(GL_LIGHT1, GL_POSITION, light1_Pos);//默认0,0,1的位置。第四个参数为1，为定点光。0平行光。聚光灯是一个特殊的定点光
	
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);//设置聚光灯方向

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 7.5f);//设置聚光灯角度。圆锥角横截面角度的一半
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 128);//聚光灯强度，中间强，周边弱

	glEnable(GL_LIGHT1);//把这个做成聚光灯

	return true;
}

bool CGfxOpenGL::Shutdown() {
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
		gluPerspective(60.0f, (GLfloat)m_windowWidth / (GLfloat)m_windowHeight, 1.0f, 1000.0f);//透视，近大远小。近裁剪面，远裁剪面。投影变换
	}
	else
	{
		glOrtho(-1.0, 1.0, -1.0, 1.0, 1, 1000.0);//正交
	}
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CGfxOpenGL::Prepare(float dt) {
}

void CGfxOpenGL::Render() {
	//3D绘图
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空背景。变黑
	glLoadIdentity();

	//gluLookAt(1.0f, 1.5f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	gluLookAt(9.0f, 9.0f, 20.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

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

	GLfloat material_yellow[4] = {0.7f, 0.6f, 0.1f, 1.0f};//rgba
	GLfloat material_white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };//rgba
	GLfloat material_ambient[4] = { 0.15f, 0.15f, 0.15f, 1.0f };//rgba
	GLfloat material_black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };//rgba
	GLfloat material_red[4] = { 0.8f, 0.0f, 0.0f, 1.0f };//rgba

	glMaterialf(GL_FRONT, GL_EMISSION, 0);//物体的正面的“自发光”设为0，不发光。
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);//material_ambient材质的环境光反射系数 将会乘以 光源的环境光 0.3 *0.5 = 0.15
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_yellow);//材质的漫反射光系数。漫反射光的计算方法比环境光稍复杂。也是乘法。网上有公式
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_black);//镜面反射光系数。系数为0，就是没有
	glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);//光泽度系数。计算镜面反射光时用到。
	//最终。自发光+环境光+漫反射光+镜面反射光=我们看到的颜色
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
		glTranslatef(-7.0f, 0.0f, 0.0f);
		DrawCube(9.0, 256);//边长，分辨率
	glPopMatrix();
/*
	
	glPointSize(15.0f);
	glBegin(GL_POINTS);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(light1_Pos[0], light1_Pos[1], light1_Pos[2]);
		glVertex3f((light1_Pos[0] + spot_dir[0]), (light1_Pos[1] + spot_dir[1]), (light1_Pos[2] + spot_dir[2]));
	glEnd();

	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex3f(-5.0f, 0.0f, 0.0f);
		glVertex3f(5.0f, 0.0f, 0.0f);

		glVertex3f(0.0f, -5.0f, 0.0f);
		glVertex3f(0.0f, 5.0f, 0.0f);

		glVertex3f(0.0f, 0.0f, -5.0f);
		glVertex3f(0.0f, 0.0f, 5.0f);



		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(spot_dir[0], spot_dir[1], spot_dir[2]);

		glVertex3f(light1_Pos[0], light1_Pos[1], light1_Pos[2]);
		glVertex3f( (light1_Pos[0] + spot_dir[0]), (light1_Pos[1] + spot_dir[1]), (light1_Pos[2] + spot_dir[2]));

		glVertex3f(light1_Pos[0], light1_Pos[1], light1_Pos[2]);
		glVertex3f(-6.0f, 7.5f, 0.0f);
	glEnd();*/
}