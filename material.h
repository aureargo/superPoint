#ifndef MATERIAL_H
#define MATERIAL_H



#define NOIR glm::vec3(0.f, 0.f, 0.f)
#define NB_RAYONS_DIFFUS 1

bool aLaLumiere(const glm::vec3& p, const glm::vec3& l);
glm::vec3 radiance (const Ray & r, int radMax);
bool refract(glm::vec3 i, glm::vec3 n, float ior, glm::vec3 &wo);
glm::vec3 reflect(const glm::vec3& i, const glm::vec3& n);
glm::vec3 sample_cos(const float u, const float v, const glm::vec3& n);
float random_u();

inline float speculaire(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, float brillance = 20)
{
    const glm::vec3 R = reflect(glm::normalize(l-p),n);
    return std::pow(fabsf(glm::dot(R,-cam.direction)), brillance);

}

struct Diffuse  {
    const glm::vec3 color;

    glm::vec3 direct(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const
    {
        if(!aLaLumiere(p,l))
            return NOIR;
        float diffuse = fabsf(glm::dot(n, glm::normalize(l-p))) / pi;
        float specular = speculaire(cam, p, n, l);
        return (color * (diffuse+specular));
    }

    glm::vec3 indirect(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const
    {
        //glm::vec3 w = reflect(-cam.direction, n);
        glm::vec3 c(0.f,0.f,0.f);
        float pdf = glm::dot(n, cam.direction);   // normalement glm::dot(n, -cam.direction) mais on inverse tout après
        glm::vec3 n2;
        if(pdf < 0)
            n2 = n;
        else
            n2 = -n;

        for(int i = 0;  i < NB_RAYONS_DIFFUS; i++)
        {
            glm::vec3 w(sample_cos(random_u(),random_u(), n2));
            c += (radiance(Ray{p,w}, radMax-1)*fabsf(glm::dot(n2,w)));
        }

        return color*(c/(float)NB_RAYONS_DIFFUS);

        //return NOIR;
        (void) cam;   (void) p;   (void) n;   (void) l;   (void) radMax;
    }
};

/*************************************************************************************/

struct Glass    {
    const glm::vec3 color;
    const float refractProp;

    glm::vec3 direct(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const
    {
        /*if(!aLaLumiere(p,l))
            return NOIR;
        float diffuse = fabsf(glm::dot(n, glm::normalize(l-p))) / pi;
        return (color * diffuse);*/

        float specular = speculaire(cam, p, n, l);
        return color*specular;
        //return NOIR;
        (void)cam;(void) p;   (void) n;   (void) l;
    }

    glm::vec3 indirect(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const
    {
        glm::vec3 wo;
        if(!refract(-cam.direction, n, refractProp, wo))
            wo = reflect(-cam.direction,n);


        glm::vec3 c = radiance(Ray{p, wo}, radMax);
        return c*color;//glm::vec3(c.r*color.r, c.g*color.g, c.b*color.b);
        //return c;
        (void) l;
    }
};

/*************************************************************************************/

struct Mirror   {
    const glm::vec3 color;

    glm::vec3 direct(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const
    {
        /*if(!aLaLumiere(p,l))
            return NOIR;
        float diffuse = fabsf(glm::dot(n, glm::normalize(l-p))) / pi;
        return (color * diffuse);*/
        float specular = speculaire(cam, p, n, l);
        return color*specular;
        //return NOIR;
        (void)cam;(void)p;(void)n;(void)l;
    }

    glm::vec3 indirect(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const
    {
        glm::vec3 newDir = reflect(-cam.direction,n);
        glm::vec3 c = radiance(Ray{p, newDir}, radMax);
        return c*color;//glm::vec3(c.r*color.r, c.g*color.g, c.b*color.b);
        (void) l;
    }
};

/*************************************************************/

template<typename T>
glm::vec3 albedo(const T &t)
{
    return t.color;
}




#endif // MATERIAL_H
