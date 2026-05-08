#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"
#include "meshUTILES.h"

//para iluminacion
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;


std::vector<Model*> modelos;


float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;



Window mainWindow;
std::vector<Shader> shaderList;

Camera camera;
Camera camaraAvatar;

Texture pisoTexture;
Texture parking;
Texture pasillo;

//Ambiente
Model express;
Model estacion;
Model hogwarts;
Model rocas1;
Model rocas2;
Model rocas3;
Model vias;
Model cancha;
Model bleachers;
Model whitecar;
Model cochesteam;
Model banca;
Model cabanaHagrid;
Model pcenter;
Model snitchBase;
Model snitchAlaDer;
Model snitchAlaIzq;
Model hatHarry;
Model pokebola;
Model libro;
Model libroAlaDer;
Model libroAlaIzq;
Model healerPokemon;


//Luces
Model lamp;
Model lamp2;
//Model lamp3;

//NPC
Model magoMalo;
Model magoBueno;
Model mismagus;
Model crow;

Model pokemon;
Model pokemon2;
Model pokemon3;

//MODELO AVATAR
Model derechaB;
Model derechaP;
Model izquiedaB;
Model izquiedaP;
Model Harry;


//SKYBOXES
Skybox skyboxDia;
Skybox skyboxNoche;

//materiales
Material Material_brillante;
Material Material_opaco;


GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

float angulovaria = 0.0f;

// luz direccional
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
int camaraAnterior = -1;

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";





void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();

	std::vector<Mesh*> meshList;
	CreateObjects(meshList);
	CreateShaders();


	//Texturas
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	parking = Texture("Textures/parking.jpg");
	parking.LoadTextureA();
	pasillo = Texture("Textures/pasillo.jpg");
	pasillo.LoadTextureA();

	//MODELOS
	express = Model();
	express.LoadModel("Models/express.obj");
	vias = Model();
	vias.LoadModel("Models/vias.obj");
	estacion = Model();
	estacion.LoadModel("Models/estacion.obj");
	cancha = Model();
	cancha.LoadModel("Models/cancha.obj");
	bleachers = Model();
	bleachers.LoadModel("Models/bleachers.obj");
	whitecar = Model();
	whitecar.LoadModel("Models/whitecar.obj");
	cochesteam = Model();
	cochesteam.LoadModel("Models/cochesteam.obj");
	hogwarts = Model();
	hogwarts.LoadModel("Models/hogwarts.obj");
	banca = Model();
	banca.LoadModel("Models/banca.obj");
	cabanaHagrid = Model();
	cabanaHagrid.LoadModel("Models/hagrid.obj");
	rocas1 = Model();
	rocas1.LoadModel("Models/rocas1.obj");
	rocas2 = Model();
	rocas2.LoadModel("Models/rocas2.obj");
	rocas3 = Model();
	rocas3.LoadModel("Models/rocas3.obj");
	pcenter = Model();
	pcenter.LoadModel("Models/pcenter.obj");
	hatHarry = Model();
	hatHarry.LoadModel("Models/hat2.obj");


	//MODELOS ANIMADOS
	snitchBase = Model();
	snitchBase.LoadModel("Models/snitchHarry.obj");
	snitchAlaDer = Model();
	snitchAlaDer.LoadModel("Models/snitchalaDerHarry.obj");
	snitchAlaIzq = Model();
	snitchAlaIzq.LoadModel("Models/snitchalaIzqHarry.obj");
	pokebola = Model();
	pokebola.LoadModel("Models/pokebola.obj");
	libro = Model();
	libro.LoadModel("Models/libro.obj");
	libroAlaDer = Model();
	libroAlaDer.LoadModel("Models/libroalaDer.obj");
	libroAlaIzq = Model();
	libroAlaIzq.LoadModel("Models/libroalaIzq.obj");

	//NPC 
	magoMalo = Model();
	magoMalo.LoadModel("Models/magoMalo.obj");
	magoBueno = Model();
	magoBueno.LoadModel("Models/magoBueno.obj");
	mismagus = Model();
	mismagus.LoadModel("Models/mismagus.obj");
	crow = Model();
	crow.LoadModel("Models/crow.obj");

	//Modelos Luces
	lamp = Model();
	lamp.LoadModel("Models/lamp.obj");
	lamp2 = Model();
	lamp2.LoadModel("Models/lamp_g.obj");
	//lamp3 = Model();
	//lamp3.LoadModel("Models/lamp3.obj");

	//AVATAR
	derechaB = Model();
	derechaB.LoadModel("Models/derechaB.obj");
	derechaP = Model();
	derechaP.LoadModel("Models/derechaP.obj");
	izquiedaB = Model();
	izquiedaB.LoadModel("Models/izquierdaB.obj");
	izquiedaP = Model();
	izquiedaP.LoadModel("Models/izquierdaP.obj");
	Harry = Model();
	Harry.LoadModel("Models/harry.obj");



	//CAMARAS
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);
	camaraAvatar = Camera(glm::vec3(2.0f, 2.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, 0.0f, 0.3f, 0.5f);


	//SKYBOXES
	std::vector<std::string> skyboxFacesNoche;
	skyboxFacesNoche.push_back("Textures/Skybox/pxN.jpg");
	skyboxFacesNoche.push_back("Textures/Skybox/nxN.jpg");
	skyboxFacesNoche.push_back("Textures/Skybox/nyN.jpg");
	skyboxFacesNoche.push_back("Textures/Skybox/pyN.jpg");
	skyboxFacesNoche.push_back("Textures/Skybox/pzN.jpg");
	skyboxFacesNoche.push_back("Textures/Skybox/nzN.jpg");
	skyboxNoche = Skybox(skyboxFacesNoche);

	std::vector<std::string> skyboxFacesDia;
	skyboxFacesDia.push_back("Textures/Skybox/px.jpg");
	skyboxFacesDia.push_back("Textures/Skybox/nx.jpg");
	skyboxFacesDia.push_back("Textures/Skybox/ny.jpg");
	skyboxFacesDia.push_back("Textures/Skybox/py.jpg");
	skyboxFacesDia.push_back("Textures/Skybox/pz.jpg");
	skyboxFacesDia.push_back("Textures/Skybox/nz.jpg");
	skyboxDia = Skybox(skyboxFacesDia);



	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	unsigned int pointLightCount = 0;

	// Posiciones de todas las lámparas "lamp3.obj" faros
	std::vector<glm::vec3> posicionesLamp = {
		glm::vec3(10.0f, 0.0f, -90.0f),
		glm::vec3(-200.0f, 0.0f, -90.0f),
		glm::vec3(240.0f, 0.0f, 90.0f),
		glm::vec3(10.0f, 0.0f, 90.0f),
		glm::vec3(-200.0f, 0.0f, 90.0f),
		glm::vec3(240.0f, 0.0f, 280.0f),
	};

	//  Inicialización de las luces de lamp.obj
	for (int i = 0; i < posicionesLamp.size(); i++) {
		pointLights[i] = PointLight(1.0f, 1.0f, 1.0f,
			0.6f, 1.0f,
			0.0f, 0.0f, 0.f,
			0.3f, 0.01f, 0.005f);
		pointLightCount++;
	}

	// luz lampara gis
	pointLights[posicionesLamp.size()] = PointLight(1.0f, 1.0f, 1.0f, // Color blanco
		0.6f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.05f, 0.005f);
	pointLightCount++;

	// luz lampara javi
	pointLights[posicionesLamp.size() + 1] = PointLight(1.0f, 1.0f, 1.0f, // Color blanco
		0.6f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.05f, 0.005f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	// Lista de posiciones calculadas para las bancas
	std::vector<glm::vec3> posicionesBancas = {
		glm::vec3(-10.0f, 6.5f, -60.0f),
		glm::vec3(-218.0f, 6.5f, -60.0f),
		glm::vec3(90.0f, 6.5f, 60.0f),
		glm::vec3(-118.0f, 6.5f, 60.0f)
	};



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);


	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
	glm::vec3 sol;
	glm::vec3 posicionModelo = glm::vec3(275.0f, 18.0f, 0.0f);

	// Posiciones de las luces
	glm::mat4 nodoLamp;
	glm::vec4 posLuzLamp;
	glm::mat4 nodoLampara1;
	glm::vec4 posLuzLamp1;
	glm::mat4 nodoLamp2;
	glm::vec4 posLuzLamp2;


	// nodos para animacion de snitch
	glm::mat4 nodoSnitchBase;
	glm::mat4 nodoSnitchAlaDer;
	glm::mat4 nodoSnitchAlaIzq;
	float movOffsetSnitch = 0.12f;
	float aleteoOffsetSnitch = 100.0f; // alas velocida


	// animacion tren
	float avanceTren = 0.0f;

	// nodos para animacion libro
	glm::mat4 nodoLibro;
	glm::mat4 nodoLibroAlaDer;
	glm::mat4 nodoLibroAlaIzq;

	// grupo 1 libros 
	float offsetLibroX = -150.0f;
	float offsetDirLibro = 1.0f;
	float offsetGiroLibro = 0.01f;
	float offsetGiroObjetivo = 0.0f;

	std::vector<glm::vec3> posicionesCopiasLibros = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 120.0f),
		glm::vec3(240.0f, 0.0f, 0.0f),
		glm::vec3(240.0f, 0.0f, 150.0f)
	};

	// grupo 2 libros (direccion opuesta) 
	float offsetLibroX2 = -50.0f;
	float offsetDirLibro2 = -1.0f;
	float offsetGiroLibro2 = 180.0f;
	float offsetGiroObjetivo2 = 180.0f;

	std::vector<glm::vec3> posicionesCopiasLibros2 = {
		glm::vec3(0.0f, 20.0f, 40.0f),
		glm::vec3(0.0f, 20.0f, 160.0f),
		glm::vec3(240.0f, 20.0f, 40.0f),
		glm::vec3(240.0f, 20.0f, 190.0f)
	};


	float luzSolar = 0.5f;
	float cambioSolar = 0.00005;
	bool dia = false;
	float anguloSol;
	float angulo;
	bool caminar;
	bool esNoche;
	float cambioDiaNoche;
	float velArticulaciones = 0.0f;
	float Articulaciones;


	while (!mainWindow.getShouldClose())
	{
		caminar = false;
		Camera* activeCamera;
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f * deltaTime;

		glfwPollEvents();

		// CAMARA /////////////////////////////////////////////////////////7
		GLfloat xChange = mainWindow.getXChange(), yChange = mainWindow.getYChange();

		int camaraActiva = mainWindow.getCamaraActiva();

		if (camaraActiva == 0)
		{
			activeCamera = &camera;
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(xChange, yChange);
		}
		else if (camaraActiva == 1)
		{
			glm::vec3 movAvatar = glm::normalize(glm::vec3(camaraAvatar.getCameraDirection().x, 0.0f, camaraAvatar.getCameraDirection().z));
			glm::vec3 vista = glm::normalize(glm::cross(movAvatar, glm::vec3(0.0f, 1.0f, 0.0f)));
			glm::vec3 camOffset = -movAvatar * 23.0f + glm::vec3(0.0f, 10.5f, 0.0f);
			glm::vec3 camPos = posicionModelo + camOffset;

			camaraAvatar.setPosition(camPos);
			activeCamera = &camaraAvatar;
			caminar = false;

			camaraAvatar.mouseControl(xChange, yChange);

			//ANIMACION AVATAR
			posicionModelo += (mainWindow.getMoverAdelante() ? movAvatar * deltaTime * 1.0f : glm::vec3(0.0f));
			posicionModelo -= (mainWindow.getMoverAtras() ? movAvatar * deltaTime * 1.0f : glm::vec3(0.0f));
			posicionModelo -= (mainWindow.getMoverIzquierda() ? vista * deltaTime * 1.0f : glm::vec3(0.0f));
			posicionModelo += (mainWindow.getMoverDerecha() ? vista * deltaTime * 1.0f : glm::vec3(0.0f));

			caminar = mainWindow.getMoverAdelante() || mainWindow.getMoverAtras() || mainWindow.getMoverIzquierda() || mainWindow.getMoverDerecha();
		}
		else
		{
			activeCamera = &camaraAvatar;
		}
		//////////////////////////////


		//Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (luzSolar >= 0.30f)
		{
			skyboxDia.DrawSkybox(activeCamera->calculateViewMatrix(), projection);
		}
		else
		{
			skyboxNoche.DrawSkybox(activeCamera->calculateViewMatrix(), projection);
		}

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//Información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(activeCamera->calculateViewMatrix()));
		glUniform3f(uniformEyePosition, activeCamera->getCameraPosition().x, activeCamera->getCameraPosition().y, activeCamera->getCameraPosition().z);

		SpotLight spotLightsToSend[MAX_SPOT_LIGHTS];
		unsigned int activeSpotLights = 0;
		for (int i = 0; i < MAX_SPOT_LIGHTS; i++) {
			if (mainWindow.getLucesSpot()[i]) {
				spotLightsToSend[activeSpotLights++] = spotLights[i];
			}
		}
		shaderList[0].SetSpotLights(spotLightsToSend, activeSpotLights);


		// DIA A NOCHE /////////////////////////////////////////////
		if (dia) {
			luzSolar -= cambioSolar;
			if (luzSolar <= 0.005f) { luzSolar = 0.005f; dia = false; }
		}
		else {
			luzSolar += cambioSolar;
			if (luzSolar >= 0.9f) { luzSolar = 0.9f; dia = true; }
		}

		anguloSol = glm::radians(180.0f * luzSolar);
		sol.x = cos(anguloSol); sol.y = -1.0f; sol.z = 0.0f;
		mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, (luzSolar * 0.3f), luzSolar, sol.x, sol.y, sol.z);
		shaderList[0].SetDirectionalLight(&mainLight);


		// Calculo de posiciones de las luces de los faros lamp.obj
		for (int i = 0; i < posicionesLamp.size(); i++)
		{
			nodoLamp = glm::mat4(1.0f);
			nodoLamp = glm::translate(nodoLamp, posicionesLamp[i]);
			nodoLamp = glm::scale(nodoLamp, glm::vec3(10.0f, 10.0f, 10.0f));
			nodoLamp = glm::rotate(nodoLamp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			posLuzLamp = nodoLamp * glm::vec4(-1.0f, 2.8f, 0.0f, 1.0f);
			pointLights[i].SetPos(glm::vec3(posLuzLamp));

		}

		// Jerarquía luz lampara javi 

		nodoLampara1 = glm::mat4(1.0f);
		nodoLampara1 = glm::translate(nodoLampara1, glm::vec3(240.0f, 0.0f, -90.0f));
		nodoLampara1 = glm::scale(nodoLampara1, glm::vec3(10.0f, 10.0f, 10.0f));
		posLuzLamp1 = nodoLampara1 * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		pointLights[posicionesLamp.size()].SetPos(glm::vec3(posLuzLamp1));

		// jerarquía luz lampara gis)
		nodoLamp2 = glm::mat4(1.0f);
		nodoLamp2 = glm::translate(nodoLamp2, glm::vec3(-185.0f, 14.0f, -214.0f));
		nodoLamp2 = glm::scale(nodoLamp2, glm::vec3(3.0f, 3.0f, 3.0f));
		posLuzLamp2 = nodoLamp2 * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		pointLights[posicionesLamp.size()].SetPos(glm::vec3(posLuzLamp2));

		//Lamparas
		esNoche = luzSolar < 0.28f;
		if (esNoche)

			shaderList[0].SetPointLights(pointLights, pointLightCount);
		else
			shaderList[0].SetPointLights(pointLights, 0);
		//////////////////////////////////////////////////////////////


		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));


		//PISO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		//////////////// HARRY ////////////////////////////////////////////////////////

		velArticulaciones += 0.5f * deltaTime;
		float Articulaciones;
		if (caminar) { Articulaciones = sin(glm::radians(velArticulaciones * 12.0f)) * glm::radians(30.0f); }
		else { Articulaciones = 0.0f; }
		angulo = atan2(camaraAvatar.getCameraDirection().x, camaraAvatar.getCameraDirection().z);
		model = glm::mat4(1.0f);
		model = glm::translate(model, posicionModelo);
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		model = glm::rotate(model, angulo, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Harry.RenderModel();

		//Pierna derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.1f, -1.2f, -0.13f));
		model = glm::rotate(model, -Articulaciones / 3, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		derechaP.RenderModel();

		//Brazo Derecho
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.36f, 0.0f, 0.0f));
		model = glm::rotate(model, Articulaciones, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		derechaB.RenderModel();

		//Pierna izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.16f, -1.29f, -0.2f));
		model = glm::rotate(model, Articulaciones / 3, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		izquiedaP.RenderModel();

		//Brazo izquierdo
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.42, 0.0, 0.0f));
		model = glm::rotate(model, -Articulaciones, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		izquiedaB.RenderModel();

		/////////////////////////////////////////////////////////////////////////////////


		//Hogwarts
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-720.0f, 0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(300.0f, 300.0f, 300.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		hogwarts.RenderModel();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-90.0f, -25.0f, 345.0f));
		model = glm::scale(model, glm::vec3(300.0f, 300.0f, 300.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rocas1.RenderModel();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-110.0, -25.0f, -315.0f));
		model = glm::scale(model, glm::vec3(300.0f, 300.0f, 300.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rocas2.RenderModel();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(390.0, -25.0f, -50.0f));
		model = glm::scale(model, glm::vec3(300.0f, 300.0f, 200.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rocas3.RenderModel();



		//ESTACIONAMIENTO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, 0.1f, 190.0f));
		modelaux = model;
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 1.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		parking.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-20.0f, 0.1f, 45.0f));
		model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		whitecar.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(24.0f, 0.1f, -45.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cochesteam.RenderModel();


		//Express
		if (avanceTren < 220.0f) {
			avanceTren += 0.12f * deltaTime;
		}
		else {
			avanceTren = 220.0f;
		}

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(324.0f, 0.1f, -220.0f + avanceTren));
		modelaux = model;
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		express.RenderModel();

		//Vias
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(324.0f, 0.1f, -20.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 2.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vias.RenderModel();

		//Estacion
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(280.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		estacion.RenderModel();

		//banca
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		for (int i = 0; i < posicionesBancas.size(); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, posicionesBancas[i]);
			if (posicionesBancas[i].z > 0) {
				model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}
			model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			banca.RenderModel();
		}

		// PASILLO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 0.1f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 27.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pasillo.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[8]->RenderMesh();



		//CANCHA POKEMON-HARRY -----------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.1f, -200.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cancha.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 10.f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bleachers.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-33.0f, 0.2f, 10.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		magoMalo.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(38.0f, 0.2f, 10.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		magoBueno.RenderModel();

		// Mismagius
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 35.0f + sin(angulovaria * 0.2f) * 0.5f, 10.0f));
		//animacion de subida y bajada
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mismagus.RenderModel();

		// Crow
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.0f, 35.0f + sin(angulovaria * 0.2f) * 0.5f, 10.0f)); //animacion de subida y bajada
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		crow.RenderModel();



		//CENTER POKEMON
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, 250.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pcenter.RenderModel();

		//healer pokemon
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, 20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		healerPokemon.RenderModel();

		// declaración de pokemones


		//POKEBOLA (animada)

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(30.0f + sin(angulovaria * 0.2f) * 2.0f, 9.0f, 62.0f)); // animacion en x 
		model = glm::rotate(model, ((sin(angulovaria * 0.2f) * 2.0f) * 50.0f) * toRadians, glm::vec3(0.0f, 0.0f, -1.0f)); //giro de la pokebola
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pokebola.RenderModel();

		//  SNITCH (animado ) -------------------------------------------
		float posX = -60.0f + (sin(angulovaria * movOffsetSnitch) * 25.0f);
		float posZ = -150.0f;
		float posY = 40.0f + (sin(angulovaria * movOffsetSnitch * 2.0f) * 8.0f);

		nodoSnitchBase = glm::mat4(1.0f);
		nodoSnitchBase = glm::translate(nodoSnitchBase, glm::vec3(posX, posY, posZ));
		nodoSnitchBase = glm::scale(nodoSnitchBase, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(nodoSnitchBase));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		snitchBase.RenderModel();

		//aleteo alas
		float anguloAleteo = sin(angulovaria * aleteoOffsetSnitch) * 35.0f;
		//ala derecha
		nodoSnitchAlaDer = nodoSnitchBase;
		nodoSnitchAlaDer = glm::translate(nodoSnitchAlaDer, glm::vec3(-0.5f, 0.5f, 0.25f));
		nodoSnitchAlaDer = glm::rotate(nodoSnitchAlaDer, anguloAleteo * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(nodoSnitchAlaDer));
		snitchAlaDer.RenderModel();
		// ala izquierda
		nodoSnitchAlaIzq = nodoSnitchBase;
		nodoSnitchAlaIzq = glm::translate(nodoSnitchAlaIzq, glm::vec3(0.5f, 0.5f, 0.25f));
		nodoSnitchAlaIzq = glm::rotate(nodoSnitchAlaIzq, -anguloAleteo * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(nodoSnitchAlaIzq));
		snitchAlaIzq.RenderModel();

		// LIBRO ANIMADO-----------------------

		// grupo 1 libros movimiento
		offsetLibroX += 0.7f * offsetDirLibro * deltaTime;
		if (offsetLibroX >= -80.0f) { //Desplazamiento máximo hacia la derecha
			offsetLibroX = -80.0f;
			offsetDirLibro = -1.0f;
			offsetGiroObjetivo = 180.0f;
		}
		else if (offsetLibroX <= -290.0f) { //Desplazamiento máximo hacia la izquierda
			offsetLibroX = -290.0f;
			offsetDirLibro = 1.0f;
			offsetGiroObjetivo = 0.0f;
		}
		offsetGiroLibro += (offsetGiroObjetivo - offsetGiroLibro) * 0.7f * deltaTime;

		// grupo 2 libros movimiento (direccion opuesta) 
		offsetLibroX2 += 0.7f * offsetDirLibro2 * deltaTime;
		if (offsetLibroX2 >= -80.0f) { //Desplazamiento máximo hacia la derecha
			offsetLibroX2 = -80.0f;
			offsetDirLibro2 = -1.0f;
			offsetGiroObjetivo2 = 180.0f;
		}
		else if (offsetLibroX2 <= -280.0f) { //desplazamiento máximo hacia la izquierda
			offsetLibroX2 = -280.0f;
			offsetDirLibro2 = 1.0f;
			offsetGiroObjetivo2 = 0.0f;
		}
		offsetGiroLibro2 += (offsetGiroObjetivo2 - offsetGiroLibro2) * 0.7f * deltaTime;


		// Renderizado grupo 1 libros
		for (int i = 0; i < posicionesCopiasLibros.size(); i++)
		{
			nodoLibro = glm::mat4(1.0f);
			nodoLibro = glm::translate(nodoLibro, glm::vec3(offsetLibroX + posicionesCopiasLibros[i].x, 45.0f + (sin(angulovaria * 0.3f) * 3.0f) + posicionesCopiasLibros[i].y, -60.0f + posicionesCopiasLibros[i].z));
			nodoLibro = glm::rotate(nodoLibro, offsetGiroLibro * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			nodoLibro = glm::scale(nodoLibro, glm::vec3(3.5f, 3.5f, 3.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(nodoLibro));
			libro.RenderModel();

			nodoLibroAlaDer = nodoLibro;
			nodoLibroAlaDer = glm::translate(nodoLibroAlaDer, glm::vec3(0.0f, 0.0f, -0.2f));
			nodoLibroAlaDer = glm::rotate(nodoLibroAlaDer, (sin(angulovaria * 0.8f) * 30.0f) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(nodoLibroAlaDer));
			libroAlaDer.RenderModel();

			nodoLibroAlaIzq = nodoLibro;
			nodoLibroAlaIzq = glm::translate(nodoLibroAlaIzq, glm::vec3(0.0f, 0.0f, 0.2f));
			nodoLibroAlaIzq = glm::rotate(nodoLibroAlaIzq, -(sin(angulovaria * 0.8f) * 30.0f) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(nodoLibroAlaIzq));
			libroAlaIzq.RenderModel();
		}

		// Renderizado grupo 2 libros
		for (int i = 0; i < posicionesCopiasLibros2.size(); i++)
		{
			nodoLibro = glm::mat4(1.0f);
			// Usamos offsetLibroX2 y offsetGiroLibro2. El desfase en Y y Z ya viene dentro de posicionesCopiasLibros2.
			nodoLibro = glm::translate(nodoLibro, glm::vec3(offsetLibroX2 + posicionesCopiasLibros2[i].x, 45.0f + (sin(angulovaria * 0.3f) * 3.0f) + posicionesCopiasLibros2[i].y, -60.0f + posicionesCopiasLibros2[i].z));
			nodoLibro = glm::rotate(nodoLibro, offsetGiroLibro2 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			nodoLibro = glm::scale(nodoLibro, glm::vec3(3.5f, 3.5f, 3.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(nodoLibro));
			libro.RenderModel();

			nodoLibroAlaDer = nodoLibro;
			nodoLibroAlaDer = glm::translate(nodoLibroAlaDer, glm::vec3(0.0f, 0.0f, -0.2f));
			nodoLibroAlaDer = glm::rotate(nodoLibroAlaDer, (sin(angulovaria * 0.8f) * 30.0f) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(nodoLibroAlaDer));
			libroAlaDer.RenderModel();

			nodoLibroAlaIzq = nodoLibro;
			nodoLibroAlaIzq = glm::translate(nodoLibroAlaIzq, glm::vec3(0.0f, 0.0f, 0.2f));
			nodoLibroAlaIzq = glm::rotate(nodoLibroAlaIzq, -(sin(angulovaria * 0.8f) * 30.0f) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(nodoLibroAlaIzq));
			libroAlaIzq.RenderModel();
		}

		// cabana de hagrid
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-200.0f, 20.f, -220.0f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		cabanaHagrid.RenderModel();

		// sombrero
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-175.0f, 12.1f, -40.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		hatHarry.RenderModel();


		// lampara gis
		model = nodoLamp2;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamp2.RenderModel();

		// lamp 1
		// dibujo de todas las copias de la lampara 
		for (int i = 0; i < posicionesLamp.size(); i++)
		{
			nodoLampara1 = glm::mat4(1.0f);
			nodoLampara1 = glm::translate(nodoLampara1, posicionesLamp[i]);
			nodoLampara1 = glm::scale(nodoLampara1, glm::vec3(10.0f, 10.0f, 10.0f));
			nodoLampara1 = glm::rotate(nodoLampara1, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			model = nodoLampara1;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			lamp.RenderModel();
		}

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}