#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat heli() { return helis; }
	GLfloat encender() { return on; }
	GLfloat encenderPez() { return onpez; }
	GLfloat carro() { return luz; }
	GLfloat pezMov() { return nadar; }
	GLfloat teclaNave() {
		return tecla;
	}
	GLfloat spotpezX() { return x; }
	GLfloat spotpezY() { return y; }
	GLfloat spotpezZ() { return z; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat helis;
	GLfloat on;
	GLfloat luz;
	GLfloat nadar;
	GLfloat onpez,x,y,z,tecla;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};
