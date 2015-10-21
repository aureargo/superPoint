#ifndef GLOBAL_H
#define GLOBAL_H



// This code is highly based on smallpt
// http://www.kevinbeason.com/smallpt/
#include <cmath>
#include <algorithm>
#include <cassert>
#include <random>
#include <memory>
#include <fstream>
#include <iostream>
#include <vector>

// GLM (vector / matrix)
#define GLM_FORCE_RADIANS

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtx/norm.hpp> //ajouté


const float pi = 3.1415927f;
const float noIntersect = std::numeric_limits<float>::infinity();

#define NOIR glm::vec3(0.f, 0.f, 0.f)
#define BLANC glm::vec3(1.f, 1.f, 1.f)

#define SQR_PRECISION 0.04f
#define AMBIANTE 0.0f
//0.03125f
#define BRILLANCE 16

#define NEAR_INTERSECT 0.1f


#endif // GLOBAL_H
