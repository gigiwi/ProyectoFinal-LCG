
/*
Práctica 7: Iluminación 1
*/
//para cargar imagen
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
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dadoTexture;

Model Blackhawk_M;
Model fish;
Model pez;
Model lamp;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
PointLight pointLights1[MAX_POINT_LIGHTS];
PointLight pointLights2[MAX_POINT_LIGHTS]; //commonvalues y shaerlight.frag
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};

	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-1.0f, -1.0f,  1.0f,	0.0f,	0.0f,		0.0f,	0.0f,	1.0f,	//0
		1.0f, -1.0f,  1.0f,		1.0f,	0.0f,		0.0f,	0.0f,	1.0f,	//1
		1.0f,  1.0f,  1.0f,		1.0f,	0.49f,		0.0f,	0.0f,	1.0f,	//2
		-1.0f,  1.0f,  1.0f,	0.0f,	0.49f,		0.0f,	0.0f,	1.0f,	//3
		// right
		//x		y		z		S		T
		1.0f, -1.0f,  1.0f,	    0.0f,  0.49f,		1.0f,	0.0f,	0.0f,
		1.0f, -1.0f,  -1.0f,	0.0f,	0.0f,		1.0f,	0.0f,	0.0f,
		1.0f,  1.0f,  -1.0f,	1.0f,	0.0f,		1.0f,	0.0f,	0.0f,
		1.0f,  1.0f,  1.0f,		1.0f,	0.49f,		1.0f,	0.0f,	0.0f,
		// back
		-1.0f, -1.0f, -1.0f,	0.0f,  0.0f,		0.0f,	0.0f,	-1.0f,
		1.0f, -1.0f, -1.0f,		1.0f,	0.0f,		0.0f,	0.0f,	-1.0f,
		1.0f,  1.0f, -1.0f,		1.0f,	0.49f,		0.0f,	0.0f,	-1.0f,
		-1.0f,  1.0f, -1.0f,	0.0f,	0.49f,		0.0f,	0.0f,	-1.0f,

		// left
		//x		y		z		S		T
		-1.0f, -1.0f,  -1.0f,	0.0f,  0.0f,		-1.0f,	0.0f,	0.0f,
		-1.0f, -1.0f,  1.0f,	0.0f,	0.49f,		-1.0f,	0.0f,	0.0f,
		-1.0f,  1.0f,  1.0f,	1.0f,	0.49f,		-1.0f,	0.0f,	0.0f,
		-1.0f,  1.0f,  -1.0f,	1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-1.0f, -1.0f,  1.0f,	0.0f,  1.0f,		0.0f,	-1.0f,	0.0f,
		1.0f,  -1.0f,  1.0f,	1.0f,	0.51f,		0.0f,	-1.0f,	0.0f,
		 1.0f,  -1.0f,  -1.0f,	1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,
		-1.0f, -1.0f,  -1.0f,	0.0f,	0.51f,		0.0f,	-1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -1.0f, 0.80f,  1.0f,	0.0f,  1.0f,		0.0f,	1.0f,	0.0f,
		 1.0f,  0.80f,  1.0f,	1.0f,	0.51f,		0.0f,	1.0f,	0.0f,
		  1.0f, 0.80f,  -1.0f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,
		 -1.0f, 0.80f,  -1.0f,	0.0f,	0.51f,		0.0f,	1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dadoTexture = Texture("Textures/agua.tga");
	dadoTexture.LoadTextureA();

	
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/nave.obj");

	fish = Model();
	fish.LoadModel("Models/fishTank.obj");
	pez = Model();
	pez.LoadModel("Models/pez.obj");
	lamp = Model();
	lamp.LoadModel("Models/lamp.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.4, 0.2, // 1) ambiental (radiacion) tienen mayor prioridad que color  2) Difusa (intesnidsadd) (color)
		0.0f, 0.0f, -1.0f); //direccion7
	//contador de luces puntuales

	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f, //color
		0.6f, 1.0f, // 1)radiancion (difusa) no cambio abarcar la intendisdad igual al circulo desde el punto central  2) se ve cada evz menos (intesnidad del colro)
		0.0f, 0.0f, 0.f,
		0.5f, 0.09f, 0.032f);
	pointLightCount++;

	pointLights[1] = PointLight(0.0f, 0.0f, 1.0f, //color
		0.6f, 1.0f, // 1)radiancion (difusa) no cambio abarcar la intendisdad igual al circulo desde el punto central  2) se ve cada evz menos (intesnidad del colro)
		0.0f, 0.0f, 0.f,
		1.0f, 0.2f, 0.9f);
	pointLightCount++;

	unsigned int pointLightCount1 = 0;

	pointLights1[0] = PointLight(1.0f, 1.0f, 1.0f, //color
		0.6f, 1.0f, // 1)radiancion (difusa) no cambio abarcar la intendisdad igual al circulo desde el punto central  2) se ve cada evz menos (intesnidad del colro)
		0.0f, 0.0f, 0.f,
		0.5f, 0.09f, 0.032f);
	pointLightCount1++;

	unsigned int pointLightCount2 = 0;

	pointLights2[0] = PointLight(0.0f, 0.0f, 1.0f, //color
		0.6f, 1.0f, // 1)radiancion (difusa) no cambio abarcar la intendisdad igual al circulo desde el punto central  2) se ve cada evz menos (intesnidad del colro)
		0.0f, 0.0f, 0.f,
		1.0f, 0.2f, 0.9f);
	pointLightCount2++;



	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f, 1.0f, 0.0f,   // amarillo
		1.0f, 2.0f,         // ilumina
		0.0f, -2.0f, 0.0f,   // posición 
		0.0f, -1.0f, 0.0f,
		1.0f, 0.1f, 0.01f,
		20.0f
	);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 0.0f, 0.0f,   // amarillo
		1.0f, 2.0f,         // ilumina
		0.0f, -2.0f, 0.0f,   // posición 
		0.0f, -1.0f, 0.0f,
		1.0f, 0.1f, 0.01f,
		5.0f
	);
	spotLightCount++;

	unsigned int spotLightCount2 = 0;
	spotLights2[0] = SpotLight(1.0f, 1.0f, 0.0f,   // amarillo
		1.0f, 2.0f,         // ilumina
		0.0f, -2.0f, 0.0f,   // posición 
		0.0f, -1.0f, 0.0f,
		1.0f, 0.1f, 0.01f,
		20.0f
	);
	spotLightCount2++;

	spotLights2[1] = SpotLight(1.0f, 0.0f, 0.0f,   // amarillo
		1.0f, 2.0f,         // ilumina
		0.0f, -2.0f, 0.0f,   // posición 
		0.0f, -1.0f, 0.0f,
		1.0f, 0.1f, 0.01f,
		5.0f
	);
	spotLightCount2++;


	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana



	glm::mat4 model(1.0);
	glm::mat4 helicop(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 pezi;
	glm::vec3 lowerLight;

	int bandera = 1;
	int bandera2 = 0;
	int banderapez = 1;
	float prev = 0.0f;
	int mov = 0; 
	float cur,x,y,z;
	float t;
	float xpez, ypez, zpez;
	float t2;

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		/*lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());*/
		//spotLights[1].SetPos(poscoche + glm::vec(x, y, cofre));

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		//lowerLight = camera.getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		//



		//spotLights[1].SetPos(poscoche + glm::vec(x, y, cofre));

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);

		bandera = mainWindow.encender();
		banderapez = mainWindow.encenderPez();
		if (bandera == 1 && banderapez == 1)
		{

			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else if (bandera == 1 && banderapez == 0)
		{
			
			shaderList[0].SetPointLights(pointLights1, pointLightCount1);
		}
		else if (bandera == 0 && banderapez == 1)	
		{
			shaderList[0].SetPointLights(pointLights2, pointLightCount2);

		}else
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount-2);
		}
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();






		//Pecera
		pezi = glm::mat4(1.0f);
		pezi = glm::scale(pezi, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = pezi;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(pezi));
		fish.RenderModel();

		pezi = modelaux;

		t = mainWindow.pezMov();
		if (t <=0.75f)
		{
			y = 1.3f-t;
			z = -2.1f+(t*3.0f);
		}
		else
		{
			t2 = t-0.75f;
			y = 1.3f-0.75f+t2;
			z = -2.1f+(0.75f*3.0f)+(t2*3.0f);
		}

		xpez = mainWindow.spotpezX();
		ypez = mainWindow.spotpezY();
		zpez = mainWindow.spotpezZ();

		pezi = glm::translate(pezi, glm::vec3(0.0f, y, z));
		pezi = glm::scale(pezi, glm::vec3(0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(pezi));
		pez.RenderModel();

		pointLights[1].SetPos(
			glm::vec3(pezi * glm::vec4(0.0f, 1.0f, 0.5f, 1.0f)) //posicion
		);
		pointLights2[0].SetPos(
			glm::vec3(pezi * glm::vec4(0.0f, 1.0f, 0.5f, 1.0f)) //posicion
		);

		spotLights[1].SetFlash(
			glm::vec3(pezi * glm::vec4(0.f, 1.0f, -0.2f, 1)),
			glm::normalize(glm::vec3(pezi * glm::vec4(xpez, ypez, zpez, 0)))
		);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		spotLights2[1].SetFlash(
			glm::vec3(pezi * glm::vec4(0.f, 1.0f, -0.2f, 1)),
			glm::normalize(glm::vec3(pezi * glm::vec4(xpez, ypez, zpez, 0)))
		);

		pezi = modelaux;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		dadoTexture.UseTexture();
		pezi = glm::scale(pezi, glm::vec3(0.95f, 1.1f, 2.9f));
		pezi = glm::translate(pezi, glm::vec3(0.0f, 1.f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(pezi));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();
		glDisable(GL_BLEND);













		cur = mainWindow.heli();
		int tecla = mainWindow.teclaNave();

		helicop = glm::mat4(1.0f);
		helicop = glm::translate(helicop, glm::vec3(cur, 5.0f, 6.0f));
		helicop = glm::scale(helicop, glm::vec3(3.0f, 3.0f, 3.0f));
		helicop = glm::rotate(helicop, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		helicop = glm::rotate(helicop, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		helicop = glm::translate(helicop, glm::vec3(0.0f, 0.0f, -5.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(helicop));
		Blackhawk_M.RenderModel();

		spotLights[0].SetFlash(
			glm::vec3(helicop * glm::vec4(0, -1, 0, 1)),
			glm::normalize(glm::vec3(helicop * glm::vec4(-0.7f, -1, 0, 0)))
		);
		spotLights2[0].SetFlash(
			glm::vec3(helicop * glm::vec4(0, -1, 0, 1)),
			glm::normalize(glm::vec3(helicop * glm::vec4(0.7f, -1, 0, 0)))
		);

		if (tecla == 1)
		{
			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}
		else if (tecla == 0)
		{
			shaderList[0].SetSpotLights(spotLights2, spotLightCount2);
		}



		


		//lamapara
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.0f, -1.0f, -6.99f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamp.RenderModel();
		pointLights[0].SetPos(
			glm::vec3(model * glm::vec4(-1.0f, 2.0f, 0.0f, 1.0f)) //posicion
		);
		pointLights1[0].SetPos(
			glm::vec3(model * glm::vec4(-1.0f, 2.0f, 0.0f, 1.0f)) //posicion
		);

		

		

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
