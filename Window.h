#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>
#include "CommonValues.h"

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

	bool getMoverAdelante() { return moverAdelante; }
	bool getMoverAtras() { return moverAtras; }
	bool getMoverIzquierda() { return moverIzquierda; }
	bool getMoverDerecha() { return moverDerecha; }
	bool getMoverTren() { return trenMover; }
	void setMoverTren(bool value);

	bool* getLucesSpot() { return lucesSpot; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	int getCamaraActiva() { return camaraActiva; }
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
	int camaraActiva;
	bool mouseFirstMoved;
	bool lucesSpot[MAX_SPOT_LIGHTS] = { false };

	bool moverAdelante;
	bool moverAtras;
	bool moverIzquierda;
	bool moverDerecha;
	bool trenMover;


	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};
