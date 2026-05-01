#pragma once
#include <vector>
#include "Mesh.h"
#include <glew.h>
#include <glfw3.h>

// Declaraciones
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount,
    GLfloat* vertices, unsigned int verticeCount,
    unsigned int vLength, unsigned int normalOffset);

void CreateObjects(std::vector<Mesh*>& meshList);
