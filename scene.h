#ifndef SCENE_H
#define SCENE_H

#include "global.h"
#include "lumiere/lumiere.h"
#include "Object/ObjectTpl.h"
#include "Object/primitive/primitive.h"

class Diffuse;
struct Object;

namespace scene
{
    // Primitives

    // Left Wall
    const Triangle leftWallA{{0, 0, 0}, {0, 100, 0}, {0, 0, 150}};
    const Triangle leftWallB{{0, 100, 150}, {0, 100, 0}, {0, 0, 150}};

    // Right Wall
    const Triangle rightWallA{{100, 0, 0}, {100, 100, 0}, {100, 0, 150}};
    const Triangle rightWallB{{100, 100, 150}, {100, 100, 0}, {100, 0, 150}};

    // Back wall
    const Triangle backWallA{{0, 0, 0}, {100, 0, 0}, {100, 100, 0}};
    const Triangle backWallB{{0, 0, 0}, {0, 100, 0}, {100, 100, 0}};

    // Bottom Floor
    const Triangle bottomWallA{{0, 0, 0}, {100, 0, 0}, {100, 0, 150}};
    const Triangle bottomWallB{{0, 0, 0}, {0, 0, 150}, {100, 0, 150}};

    // Top Ceiling
    const Triangle topWallA{{0, 100, 0}, {100, 100, 0}, {0, 100, 150}};
    const Triangle topWallB{{100, 100, 150}, {100, 100, 0}, {0, 100, 150}};

    const Sphere leftSphere{16.5, glm::vec3 {27, 16.5, 47}};
    const Sphere rightSphere{16.5, glm::vec3 {73, 16.5, 78}};

    /*******************************************************/
    const Sphere Sphere3{6.5, glm::vec3 {27, 14.5, 108}};
    const Sphere Sphere4{18.5, glm::vec3 {78, 64.5, 88}};
    const Sphere Sphere5{15, glm::vec3(27,60,60)};
    /*******************************************************/

    const Mesh mesh1 = [](){
        Mesh ret(glm::vec3(50,0,70), "./Beautiful Girl.obj");
        ret.rotate(pi,glm::vec3(0,1,0));
        ret.scale(0.7);
        return ret;
    }();

    /*******************************************************/

    const Lumiere light({50.0, 70.0, 81.6});
    const Lumiere light2({50.0, 30.0, 150.0}, {.5,1.0,0.1});
    const Lumiere light3({20.0, 50.0, 75.0}, {1.0,0.1,0.5});
    const Lumiere light4({80.0, 50.0, 75.0}, {0.1,0.5,1.0});

    // Materials
    const Diffuse white{{.75, .75, .75}};
    const Diffuse red{{.75, .25, .25}};
    const Diffuse blue{{.25, .25, .75}};
    const Diffuse yellow{{.75, .75, .25}};
    const Diffuse green{{.25, .75, .25}};
    const Diffuse grey{{.5,.5,.5}};

    const Glass glass{{1,1,1},1.33};
    const Glass glass2{{1.0, .3, 1.0}, 4.0};
    const Mirror mirror{{.9, .9, .3}};
    const Mirror mirror2{{.4, 1.0, .4}};

    // Objects
    // Note: this is a rather convoluted way of initialising a vector of unique_ptr ;)
    const std::vector<std::unique_ptr<Object>> objects = [] (){
        std::vector<std::unique_ptr<Object>> ret;
        ret.push_back(makeObject(backWallA, yellow));
        ret.push_back(makeObject(backWallB, yellow));
        ret.push_back(makeObject(topWallA, green));
        ret.push_back(makeObject(topWallB, green));
        ret.push_back(makeObject(bottomWallA, white));
        ret.push_back(makeObject(bottomWallB, white));
        ret.push_back(makeObject(rightWallA, blue));
        ret.push_back(makeObject(rightWallB, blue));
        ret.push_back(makeObject(leftWallA, red));
        ret.push_back(makeObject(leftWallB, red));

        ret.push_back(makeObject(leftSphere, mirror));
        ret.push_back(makeObject(rightSphere, glass));
        ret.push_back(makeObject(Sphere3, glass2));
        ret.push_back(makeObject(Sphere4, mirror2));
        ret.push_back(makeObject(Sphere5, white));

        ret.push_back(makeObject(mesh1, grey));

        return ret;
    }();
}

#endif // SCENE_H
