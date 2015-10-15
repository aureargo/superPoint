#ifndef LUMIERE_H
#define LUMIERE_H

struct lux   {
    float puissance;
    glm::vec3 pos;

    bool operator() (lux const& lhs, lux const& rhs) const
    {
        if(lhs.pos.y < rhs.pos.y)	return true;
        if(lhs.pos.y > rhs.pos.y)		return false;
        if(lhs.pos.x < rhs.pos.x)	return true;
        if(lhs.pos.x > rhs.pos.x)		return false;
        return (lhs.pos.z < rhs.pos.z);
    }

    size_t operator()(const lux& v) const{
        return std::hash<int>()(v.pos.x) ^ std::hash<int>()(v.pos.y) ^ std::hash<int>()(v.pos.z);
    }

    bool operator< (lux const& v) const
    {
        if(this->pos.y < v.pos.y)	return true;
        if(this->pos.y > v.pos.y)		return false;
        if(this->pos.x < v.pos.x)	return true;
        if(this->pos.x > v.pos.x)		return false;
        return (this->pos.z < v.pos.z);
    }
};

#endif // LUMIERE_H
