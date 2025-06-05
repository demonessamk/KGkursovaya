#include "Render.h"
#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "GUItextRectangle.h"
#include "MyShaders.h"
#include "Texture.h"



#include "ObjLoader.h"


#include "debout.h"



//внутренняя логика "движка"
#include "MyOGL.h"
extern OpenGL gl;
#include "Light.h"
Light light;
#include "Camera.h"
Camera camera;


bool texturing = true;
bool lightning = false;
bool alpha = false;



void draw3DBase(float _x, float _y, float z, float width, float height, float depth, float color1, float color2, float color3) {

	float x = _x - 10;
	float y = _y - 6;


	// Верхняя грань основания
	glBegin(GL_QUADS);
	glColor3f(color1, color2, color3);
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x, y, z + depth);
	glEnd();

	// Передняя грань
	glBegin(GL_QUADS);
	glColor3f(color1, color2, color3);
	glVertex3f(x, y, z + depth);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x, y + height, z + depth);
	glEnd();

	// Задняя грань
	glBegin(GL_QUADS);
	glColor3f(color1 - 0.2, color2 - 0.2, color3 - 0.2);
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x, y + height, z);
	glEnd();

	// Правая боковая грань
	glBegin(GL_QUADS);
	glColor3f(color1 - 0.2, color2 - 0.2, color3 - 0.2);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y + height, z);
	glEnd();

	// Левая боковая грань
	glBegin(GL_QUADS);
	glColor3f(color1, color2, color3);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + depth);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x, y + height, z);
	glEnd();

	// Нижняя грань
	glBegin(GL_QUADS);
	glColor3f(color1-0.2, color2 - 0.2, color3 - 0.2);
	glVertex3f(x, y + height, z);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x, y + height, z + depth);
	glEnd();


	// Окантовка всех граней
	glDisable(GL_LINE_STIPPLE);
	glLineWidth(2.0f);
	glColor3f(0.0f, 0.0f, 0.0f);

	// Окантовка передней грани
	glBegin(GL_LINE_LOOP);
	glVertex3f(x, y, z + depth);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x, y + height, z + depth);
	glEnd();

	// Окантовка задней грани
	glBegin(GL_LINE_LOOP);
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x, y + height, z);
	glEnd();

	// Окантовка верхней грани
	glBegin(GL_LINE_LOOP);
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x, y, z + depth);
	glEnd();

	// Окантовка нижней грани
	glBegin(GL_LINE_LOOP);
	glVertex3f(x, y + height, z);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x, y + height, z + depth);
	glEnd();

	// Окантовка правой грани
	glBegin(GL_LINE_LOOP);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y + height, z);
	glEnd();

	// Окантовка левой грани
	glBegin(GL_LINE_LOOP);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + depth);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x, y + height, z);
	glEnd();
}




void drawMonitor() {
	// Основание подставки (непрозрачное)
	glPushMatrix();
	glTranslatef(-7.5, 21, 2);
	glRotatef(270, 1, 0, 0);
	draw3DBase(10, 6, -7.5, 15, 2, 15, 0.93f, 0.81f, 0.86f);
	glPopMatrix();

	// Стойка (непрозрачная)
	glPushMatrix();
	glTranslatef(-1.5, 21, 2.1);
	glRotatef(90, 1, 0, 0);
	draw3DBase(10, 6, -1.5, 3, 14, 3, 0.79f, 0.89f, 0.93f);
	glPopMatrix();

	// Корпус монитора (непрозрачный)
	glPushMatrix();
	glTranslatef(-13.6, 13, 16);
	glRotatef(90, 1, 0, 0);
	draw3DBase(10, 6, -9.0, 27.2, 15.3, 2, 0.99f, 0.71f, 0.76f);
	glPopMatrix();

	// Экран (полупрозрачный) - альтернативная реализация
	glPushMatrix();
	glTranslatef(-13.1, 12, 16.5);
	glRotatef(90, 1, 0, 0);

	// Включаем прозрачность
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Рисуем полупрозрачный прямоугольник вручную
	float x = 0, y = 0, z = -7.8;
	float width = 26.2, height = 14.3, depth = 1;


	glColor4f(0.69f, 0.88f, 0.90f, 0.5f); // RGBA с альфа=0.5

	// Передняя грань
	glBegin(GL_QUADS);
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x, y + height, z);
	glEnd();

	// Отключаем прозрачность
	glDisable(GL_BLEND);
	glPopMatrix();
}



class klav {
public:
	std::chrono::steady_clock::time_point pressStartTime;
	int id;
	Texture texture;
	float pressOffset = 0; // Текущее смещение при нажатии
	bool isPressed = false;
	float pressSpeed = 0.1; // Скорость анимации нажатия
	float maxPressDepth = 0.5; // Максимальная глубина нажатия
	unsigned int texId = 0;
	klav(int _id = -1) : id(_id) {}


	void press() {
		isPressed = true;
		pressStartTime = std::chrono::steady_clock::now();
	}

	void update() {
		auto now = std::chrono::steady_clock::now();
		float elapsed = std::chrono::duration<float>(now - pressStartTime).count();

		if (!isPressed) {
			// Анимация нажатия (200ms)
			if (!pressOffset == 0)
			{
				if (elapsed < 0.2f) {
					pressOffset = -maxPressDepth * (elapsed / 0.2f);
				}
			}

		}
		else {
			// Анимация отпускания (200ms)
			if (elapsed < 0.2f) {
				pressOffset = -maxPressDepth * (1.0f - elapsed / 0.2f);
			}
			else {
				pressOffset = 0.0f;
			}
		}
	}


	void draw3Dkl(float _x, float _y, float z, float width, float height, float depth, float color1, float color2, float color3) {

		float x = _x - 10;
		float y = _y - 6;

		z += pressOffset;
		depth += pressOffset;


		// Верхняя грань основания
		glBegin(GL_QUADS);
		glColor3f(color1, color2, color3);
		glVertex3f(x, y, z);
		glVertex3f(x + width, y, z);
		glVertex3f(x + width, y, z - depth);
		glVertex3f(x, y, z - depth);
		glEnd();


		// Задняя грань
		glBegin(GL_QUADS);
		glColor3f(color1 - 0.2, color2 - 0.2, color3 - 0.2);
		glVertex3f(x, y, z - depth);
		glVertex3f(x + width, y, z - depth);
		glVertex3f(x + width, y + height, z - depth);
		glVertex3f(x, y + height, z - depth);
		glEnd();

		// Передняя грань с текстурой
		if (id >= 0) {

			glEnable(GL_TEXTURE_2D);
			switch (id) {
			case(0):
				texture.LoadTexture("textures/key0.jpg");
				texId = texture.GetID();
				break;
			case(1):
				texture.LoadTexture("textures/key1.jpg");
				texId = texture.GetID();
				break;
			case(2):
				texture.LoadTexture("textures/key2.jpg");
				texId = texture.GetID();
				break;
			case(3):
				texture.LoadTexture("textures/key3.jpg");
				texId = texture.GetID();
				break;
			case(4):
				texture.LoadTexture("textures/key4.jpg");
				texId = texture.GetID();
				break;
			case(5):
				texture.LoadTexture("textures/key5.jpg");
				texId = texture.GetID();
				break;
			case(6):
				texture.LoadTexture("textures/key6.jpg");
				texId = texture.GetID();
				break;
			case(7):
				texture.LoadTexture("textures/key7.jpg");
				texId = texture.GetID();
				break;
			case(8):
				texture.LoadTexture("textures/probel.jpg");
				texId = texture.GetID();
				break;
			case(9):
				texture.LoadTexture("textures/enter1.jpg");
				texId = texture.GetID();
				break;
			case(10):
				texture.LoadTexture("textures/enter2.jpg");
				texId = texture.GetID();
				break;
			}

			glBindTexture(GL_TEXTURE_2D, texId);

			
			glBegin(GL_QUADS);
			glColor3f(1, 1, 1); // Белый цвет для чистого отображения текстуры
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
		else {
			// Передняя грань без текстуры (если текстура не загружена)
			glBegin(GL_QUADS);
			glColor3f(color1, color2, color3);
			glVertex3f(x, y, z);
			glVertex3f(x + width, y, z);
			glVertex3f(x + width, y + height, z);
			glVertex3f(x, y + height, z);
			glEnd();
		}

		// Правая боковая грань
		glBegin(GL_QUADS);
		glColor3f(color1 - 0.2, color2 - 0.2, color3 - 0.2);
		glVertex3f(x + width, y, z);
		glVertex3f(x + width, y, z - depth);
		glVertex3f(x + width, y + height, z - depth);
		glVertex3f(x + width, y + height, z);
		glEnd();

		// Левая боковая грань
		glBegin(GL_QUADS);
		glColor3f(color1, color2, color3);
		glVertex3f(x, y, z);
		glVertex3f(x, y, z - depth);
		glVertex3f(x, y + height, z - depth);
		glVertex3f(x, y + height, z);
		glEnd();

		// Верхняя грань
		glBegin(GL_QUADS);
		glColor3f(color1 - 0.2, color2 - 0.2, color3 - 0.2);
		glVertex3f(x, y + height, z);
		glVertex3f(x + width, y + height, z);
		glVertex3f(x + width, y + height, z - depth);
		glVertex3f(x, y + height, z - depth);
		glEnd();


		// Окантовка всех граней
		glDisable(GL_LINE_STIPPLE);
		glLineWidth(2.0f);
		glColor3f(0.0f, 0.0f, 0.0f);

		// Окантовка передней грани
		glBegin(GL_LINE_LOOP);
		glVertex3f(x, y, z - depth);
		glVertex3f(x + width, y, z - depth);
		glVertex3f(x + width, y + height, z - depth);
		glVertex3f(x, y + height, z - depth);
		glEnd();

		// Окантовка задней грани
		glBegin(GL_LINE_LOOP);
		glVertex3f(x, y, z);
		glVertex3f(x + width, y, z);
		glVertex3f(x + width, y + height, z);
		glVertex3f(x, y + height, z);
		glEnd();

		// Окантовка верхней грани
		glBegin(GL_LINE_LOOP);
		glVertex3f(x, y, z);
		glVertex3f(x + width, y, z);
		glVertex3f(x + width, y, z - depth);
		glVertex3f(x, y, z - depth);
		glEnd();

		// Окантовка нижней грани
		glBegin(GL_LINE_LOOP);
		glVertex3f(x, y + height, z);
		glVertex3f(x + width, y + height, z);
		glVertex3f(x + width, y + height, z - depth);
		glVertex3f(x, y + height, z - depth);
		glEnd();

		// Окантовка правой грани
		glBegin(GL_LINE_LOOP);
		glVertex3f(x + width, y, z);
		glVertex3f(x + width, y, z - depth);
		glVertex3f(x + width, y + height, z - depth);
		glVertex3f(x + width, y + height, z);
		glEnd();

		// Окантовка левой грани
		glBegin(GL_LINE_LOOP);
		glVertex3f(x, y, z);
		glVertex3f(x, y, z - depth);
		glVertex3f(x, y + height, z - depth);
		glVertex3f(x, y + height, z);
		glEnd();
	}
};



std::vector<klav> keys;
void initKeyboard() {


	keys.clear();

	for (int i = 0; i < 4; i++) {
		klav key(i);
		keys.push_back(key);
	}
	for (int i = 0; i < 4; i++) {
		klav key(i + 4);
		keys.push_back(key);
	}
	klav probel(8);
	keys.push_back(probel);

	klav enter_part1(9);
	keys.push_back(enter_part1);
	klav enter_part2(10);
	keys.push_back(enter_part2);
}

void display() {
	draw3DBase(2, 0, 0, 16, 11, 5, 0.79f, 0.71f, 0.89f);

	draw3DBase(4, 2, 5.02, 12, 7, 2, 0.82f, 0.90f, 0.94f);


	draw3DBase(4.33333333333333, 8.33333333333333, 7.03, 0.33333333333333, 0.33333333333333, 0.5, 1, 1, 1);


	


	float kl_shir = 1;
	float kl_vis = 1;
	float kl_glub = 0.5;
	float kl_rast = 1;
	float nachalo_x = 5;
	float kl_verh_rast = 7;
	float kl_niz_rast = 5;
	float kl_vis_ot_niza = 7.53;

	// Верхний ряд
	for (int i = 0; i < 4; i++) {
		keys[i].draw3Dkl(nachalo_x + i * (kl_shir + kl_rast), kl_verh_rast, kl_vis_ot_niza, kl_shir, kl_vis, kl_glub, 0.98f, 0.85f, 0.87f);
	}

	// Нижний ряд
	for (int i = 0; i < 4; i++) {
		keys[i + 4].draw3Dkl(nachalo_x + i * (kl_shir + kl_rast), kl_niz_rast, kl_vis_ot_niza, kl_shir, kl_vis, kl_glub, 0.98f, 0.85f, 0.87f);
	}


	keys[8].draw3Dkl(6, 3, 7.53, 5, 1, 0.5, 0.77f, 0.88f, 0.71f);  // Пробел
	keys[9].draw3Dkl(13, 5, 7.53, 1, 3, 0.5, 0.77f, 0.88f, 0.71f); // Enter 1
	keys[10].draw3Dkl(14, 5, 7.53, 1, 1, 0.5, 0.77f, 0.88f, 0.71f); // Enter 2
}


void updateKeyboard() {
	for (auto& key : keys) {
		key.update();
	}
}



//переключение режимов освещения, текстурирования, альфаналожения
void switchModes(OpenGL *sender, KeyEventArg arg)
{
	//конвертируем код клавиши в букву
	auto key = LOWORD(MapVirtualKeyA(arg.key, MAPVK_VK_TO_CHAR));

	if (key >= '0' && key <= '7') {
		int idx = key - '0';
		if (idx < keys.size()) {
			keys[idx].press();
		}
	}

	if (key == ' ') {
		int idx = '8' - '0';
		if (idx < keys.size()) {
			keys[idx].press();
		}
	}

	if (key == VK_RETURN) {
		keys[9].press();
		keys[10].press();
	}

	

	switch (key)
	{
	case 'L':
		lightning = !lightning;
		break;
	case 'T':
		texturing = !texturing;
		break;
	case 'A':
		alpha = !alpha;
		break;
	}

}

//умножение матриц c[M1][N1] = a[M1][N1] * b[M2][N2]
template<typename T, int M1, int N1, int M2, int N2>
void MatrixMultiply(const T* a, const T* b, T* c)
{
	for (int i = 0; i < M1; ++i)
	{
		for (int j = 0; j < N2; ++j)
		{
			c[i * N2 + j] = T(0);
			for (int k = 0; k < N1; ++k)
			{
				c[i * N2 + j] += a[i * N1 + k] * b[k * N2 + j];
			}
		}
	}
}

//Текстовый прямоугольничек в верхнем правом углу.
//OGL не предоставляет возможности для хранения текста
//внутри этого класса создается картинка с текстом (через виндовый GDI),
//в виде текстуры накладывается на прямоугольник и рисуется на экране.
//Это самый простой способ что то написать на экране
//но ооооочень не оптимальный
GuiTextRectangle text;

//айдишник для текстуры

//выполняется один раз перед первым рендером

ObjModel f;


Shader cassini_sh;
Shader phong_sh;
Shader vb_sh;
Shader simple_texture_sh;

void initRender()
{




	initKeyboard();
	cassini_sh.VshaderFileName = "shaders/v.vert";
	cassini_sh.FshaderFileName = "shaders/cassini.frag";
	cassini_sh.LoadShaderFromFile();
	cassini_sh.Compile();

	phong_sh.VshaderFileName = "shaders/v.vert";
	phong_sh.FshaderFileName = "shaders/light.frag";
	phong_sh.LoadShaderFromFile();
	phong_sh.Compile();

	vb_sh.VshaderFileName = "shaders/v.vert";
	vb_sh.FshaderFileName = "shaders/vb.frag";
	vb_sh.LoadShaderFromFile();
	vb_sh.Compile();

	simple_texture_sh.VshaderFileName = "shaders/v.vert";
	simple_texture_sh.FshaderFileName = "shaders/textureShader.frag";
	simple_texture_sh.LoadShaderFromFile();
	simple_texture_sh.Compile();


	f.LoadModel("models//monkey.obj_m");
	//==============НАСТРОЙКА ТЕКСТУР================
	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	

	//================НАСТРОЙКА КАМЕРЫ======================
	camera.caclulateCameraPos();

	//привязываем камеру к событиям "движка"
	gl.WheelEvent.reaction(&camera, &Camera::Zoom);
	gl.MouseMovieEvent.reaction(&camera, &Camera::MouseMovie);
	gl.MouseLeaveEvent.reaction(&camera, &Camera::MouseLeave);
	gl.MouseLdownEvent.reaction(&camera, &Camera::MouseStartDrag);
	gl.MouseLupEvent.reaction(&camera, &Camera::MouseStopDrag);
	//==============НАСТРОЙКА СВЕТА===========================
	//привязываем свет к событиям "движка"
	gl.MouseMovieEvent.reaction(&light, &Light::MoveLight);
	gl.KeyDownEvent.reaction(&light, &Light::StartDrug);
	gl.KeyUpEvent.reaction(&light, &Light::StopDrug);
	//========================================================
	//====================Прочее==============================
	gl.KeyDownEvent.reaction(switchModes);
	text.setSize(512, 180);
	//========================================================
	   

	camera.setPosition(2, 1.5, 1.5);
	
}
float view_matrix[16];
double full_time = 0;
int location = 0;
void Render(double delta_time)
{    


	full_time += delta_time;
	
	//натройка камеры и света
	//в этих функциях находятся OGLные функции
	//которые устанавливают параметры источника света
	//и моделвью матрицу, связанные с камерой.

	if (gl.isKeyPressed('F')) //если нажата F - свет из камеры
	{
		light.SetPosition(camera.x(), camera.y(), camera.z());
	}
	camera.SetUpCamera();
	//забираем моделвью матрицу сразу после установки камера
	//так как в ней отсуствуют трансформации glRotate...
	//она, фактически, является видовой.
	glGetFloatv(GL_MODELVIEW_MATRIX,view_matrix);

	

	light.SetUpLight();

	//рисуем оси
	gl.DrawAxes();

	

	glBindTexture(GL_TEXTURE_2D, 0);

	//включаем нормализацию нормалей
	//чтобв glScaled не влияли на них.

	glEnable(GL_NORMALIZE);  
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	
	//включаем режимы, в зависимости от нажания клавиш. см void switchModes(OpenGL *sender, KeyEventArg arg)
	if (lightning)
		glEnable(GL_LIGHTING);
	if (texturing)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0); //сбрасываем текущую текстуру
	}
		
	if (alpha)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
		
	//=============НАСТРОЙКА МАТЕРИАЛА==============


	//настройка материала, все что рисуется ниже будет иметь этот метериал.
	//массивы с настройками материала
	float  amb[] = { 0.2, 0.2, 0.1, 1. };
	float dif[] = { 0.4, 0.65, 0.5, 1. };
	float spec[] = { 0.9, 0.8, 0.3, 1. };
	float sh = 0.2f * 256;

	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); 
	//размер блика
	glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//чтоб было красиво, без квадратиков (сглаживание освещения)
	glShadeModel(GL_SMOOTH); //закраска по Гуро      
			   //(GL_SMOOTH - плоская закраска)

	//============ РИСОВАТЬ ТУТ ==============
	
	display();

	updateKeyboard();

	drawMonitor();

	
	//сбрасываем все трансформации
	glLoadIdentity();
	camera.SetUpCamera();	
	Shader::DontUseShaders();
	//рисуем источник света
	light.DrawLightGizmo();

	//================Сообщение в верхнем левом углу=======================
	glActiveTexture(GL_TEXTURE0);
	//переключаемся на матрицу проекции
	glMatrixMode(GL_PROJECTION);
	//сохраняем текущую матрицу проекции с перспективным преобразованием
	glPushMatrix();
	//загружаем единичную матрицу в матрицу проекции
	glLoadIdentity();

	//устанавливаем матрицу паралельной проекции
	glOrtho(0, gl.getWidth() - 1, 0, gl.getHeight() - 1, 0, 1);

	//переключаемся на моделвью матрицу
	glMatrixMode(GL_MODELVIEW);
	//сохраняем матрицу
	glPushMatrix();
    //сбразываем все трансформации и настройки камеры загрузкой единичной матрицы
	glLoadIdentity();

	//отрисованное тут будет визуалзироватся в 2д системе координат
	//нижний левый угол окна - точка (0,0)
	//верхний правый угол (ширина_окна - 1, высота_окна - 1)

	
	std::wstringstream ss;
	ss << std::fixed << std::setprecision(3);
	ss << "T - " << (texturing ? L"[вкл]выкл  " : L" вкл[выкл] ") << L"текстур" << std::endl;
	ss << "L - " << (lightning ? L"[вкл]выкл  " : L" вкл[выкл] ") << L"освещение" << std::endl;
	ss << "A - " << (alpha ? L"[вкл]выкл  " : L" вкл[выкл] ") << L"альфа-наложение" << std::endl;
	ss << L"F - Свет из камеры" << std::endl;
	ss << L"G - двигать свет по горизонтали" << std::endl;
	ss << L"G+ЛКМ двигать свет по вертекали" << std::endl;
	ss << L"Коорд. света: (" << std::setw(7) <<  light.x() << "," << std::setw(7) << light.y() << "," << std::setw(7) << light.z() << ")" << std::endl;
	ss << L"Коорд. камеры: (" << std::setw(7) << camera.x() << "," << std::setw(7) << camera.y() << "," << std::setw(7) << camera.z() << ")" << std::endl;
	ss << L"Параметры камеры: R=" << std::setw(7) << camera.distance() << ",fi1=" << std::setw(7) << camera.fi1() << ",fi2=" << std::setw(7) << camera.fi2() << std::endl;
	ss << L"delta_time: " << std::setprecision(5)<< delta_time << std::endl;
	ss << L"full_time: " << std::setprecision(2) << full_time << std::endl;

	
	text.setPosition(10, gl.getHeight() - 10 - 180);
	text.setText(ss.str().c_str());
	
	text.Draw();

	//восстанавливаем матрицу проекции на перспективу, которую сохраняли ранее.
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	

	
}   



