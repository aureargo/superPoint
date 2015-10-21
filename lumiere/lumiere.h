#ifndef LUMIERE_H
#define LUMIERE_H

#include "global.h"

class Lumiere
{
public:
    Lumiere(const glm::vec3& origin);

    glm::vec3 origin;
    glm::vec3 color = glm::vec3(1,1,1);

    float absDiffus = 1.f;  //quantité absorbé à chaque fois que ça
    float absSpeculaire = 1.f;  //quatité absorbé en spéculaire
};

#endif // LUMIERE_H
