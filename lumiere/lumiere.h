#ifndef LUMIERE_H
#define LUMIERE_H

#include "global.h"

class Lumiere
{
public:
    Lumiere(const glm::vec3& origin):
            Lumiere(origin, glm::vec3(1,1,1))
    {
    }

    Lumiere(const glm::vec3& origin, const glm::vec3& color, float absDiffus = 1, float absSpeculaire = 1, float rayonMonteCarlo = 4):
            origin(origin), color(color), absDiffus(absDiffus), absSpeculaire(absSpeculaire),   rayonMonteCarlo(rayonMonteCarlo)
    {
    }

    glm::vec3 origin;
    glm::vec3 color;

    float absDiffus = 1.f;  //quantité absorbé à chaque fois que ça
    float absSpeculaire = 1.f;  //quatité absorbé en spéculaire
    float rayonMonteCarlo = 4.f;    //demi-sphere autour de laquelle on projète le rayon de lumière
};

#endif // LUMIERE_H
