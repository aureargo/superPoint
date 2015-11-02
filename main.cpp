#include "smallpt.h"
#include "scene.h"
#include <QTime>

///à déplacer dans intersect.cpp quand ce fichier pourra atteindre "scene::objects"
// WARNING: ASSUME NORMALIZED RAY
// Compute the intersection ray / scene.
// Returns true if intersection
// t is defined as the abscisce along the ray (i.e
//             p = r.o + t * r.d
// id is the id of the intersected object
Object* intersect (const Ray & r, float &t, int& id)
{
    t = noIntersect;
    Object *ret = nullptr;

    for(auto &object : scene::objects)
    {
        int i;
        float d = object->intersect(r, i);
        //if (isIntersect(d) && d < t)
        if (d < t && d > NEAR_INTERSECT)  //pas d>=0.0f pour permettre la diffusion à partir d'un objet de la scène sans que la fonction retourne le même objet
        {
            t = d;
            id = i;
            ret = object.get();
        }
    }

    return ret;
}


int main (int, char **)
{
    QTime timer;
    timer.start();

    //int w = 768, h = 768;
    int w = 640, h = 640;
    std::vector<glm::vec3> colors(w * h, glm::vec3{0.f, 0.f, 0.f});

    Ray cam {{50, 52, 295.6}, glm::normalize(glm::vec3{0, -0.042612, -1})};	// cam pos, dir
    float near = 1.f;
    float far = 10000.f;

    std::vector<Lumiere> lights;
    lights.push_back(scene::light);
    lights.push_back(scene::light2);
    lights.push_back(scene::light3);
    lights.push_back(scene::light4);

    /*std::vector<lux> lums;
    lums.reserve(10000);

    for(const auto& light:  lights)
    {
        for(int i = 0;  i < 10000;  i++)
        {
            float puissance = 2.f;
            glm::vec3 dir(random_u(), random_u(), random_u());
            dir = glm::normalize(dir);
            float t;
            Ray ray(light, dir);
            Object* obj = intersect(ray ,t);
            if(obj != nullptr)
            {
                float newPuissance = puissance*0.5;
                if(newPuissance > 0.01f)
                {
                    glm::vec3 pos = light+dir*t;
                    lums.push_back(lux{newPuissance,pos});

                    ray = Ray(pos, obj->projection(ray, obj->getNormal(pos)));
                    while(newPuissance > 0.01f)
                    {
                        obj = intersect(ray,t);
                        if(obj != nullptr)
                        {
                            newPuissance = newPuissance*0.5;
                            if(newPuissance > 0.01f)
                            {
                                pos = pos+dir*t;
                                lums.push_back(lux{newPuissance,pos});
                                ray = Ray(pos, obj->projection(ray, obj->getNormal(pos)));
                            }
                        }
                        else
                            break;
                    }
                }
            }
        }
    }

    lums.shrink_to_fit();
    std::sort(lums.begin(),	lums.end());*/


/**************************************************************************************************************/


    glm::mat4 camera =
        glm::scale(glm::mat4(1.f), glm::vec3(float(w), float(h), 1.f))
        * glm::translate(glm::mat4(1.f), glm::vec3(0.5, 0.5, 0.f))
        * glm::perspective(float(54.5f * pi / 180.f), float(w) / float(h), near, far)
        * glm::lookAt(cam.origin, cam.origin + cam.direction, glm::vec3(0, 1, 0))
        ;

    glm::mat4 screenToRay = glm::inverse(camera);




    const unsigned int  antiAliasing = 16,
                        recursionRayon = 7; //7 est bien
    int prct, prct2 = -1;
    int py = 0;

    #ifdef QT_NO_DEBUG
    #pragma omp parallel for
    #endif
    for (int y = 0; y < h; y++)
    {
        prct = 100 * py / (h - 1);
        if(prct != prct2)
        {
            std::cerr << "\rRendering: " << prct << "%";
            prct2 = prct;
        }

        /*#ifdef QT_NO_DEBUG
        #pragma omp parallel for
        #endif*/
        for (unsigned short x = 0; x < w; x++)
        {
            glm::vec4 p0 = screenToRay * glm::vec4{float(x), float(h - y), 0.f, 1.f};
            glm::vec4 p1 = screenToRay * glm::vec4{float(x), float(h - y), 1.f, 1.f};

            glm::vec3 pp0 = glm::vec3(p0 / p0.w);
            glm::vec3 pp1 = glm::vec3(p1 / p1.w);

            glm::vec3 d = glm::normalize(pp1 - pp0);
            glm::vec3 r(0,0,0);

            for(unsigned int i = 0; i < antiAliasing;  i++) {
                for(const auto& l : lights) {
                    glm::vec3 d2;
                    if(antiAliasing == 1)
                        d2 = d;
                    else
                    {
                        d2 = sample_cos(random_u(),random_u(), d);
                        d2 *= 0.001f;//glm::vec3(w,h,w*h);
                        d2 += d;
                        d2 = glm::normalize(d2);
                    }
                    r += radiance (Ray(pp0, d2), l, recursionRayon);
                }
            }
            r /= (antiAliasing*lights.size());
            colors[y * w + x] += glm::clamp(r, glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
        }
        py++;
    }

    //convolution
    for(int i = 0;  i < 0;  i++)
        colors = convolution(colors, h, w);


    std::fstream f("image.ppm", std::fstream::out);
    f << "P3\n" << w << " " << h << std::endl << "255" << std::endl;

    for (auto c : colors)
        f << toInt(c.x) << " " << toInt(c.y) << " " << toInt(c.z) << " ";


    int time = timer.elapsed();
    std::cout << std::endl << time/1000 << "." << time%1000<< " secondes" << std::endl;

}
