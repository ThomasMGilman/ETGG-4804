#pragma once

#include "math3d.h"

class Sphere{
    public:
    vec3 c;     //center
    float r;    //radius
    vec3 color;
	vec3 minPoint, maxPoint;
    float alpha;
    float refl;
    Sphere(vec3 c, float r, vec3 color, float a, float ref){
        this->c=c;
        this->r=r;
        this->color=color;
        alpha=a;
        refl=ref;

		for (int i = 0; i < this->c.len(); i++)
		{
			minPoint[i] = this->c[i] - this->r;
			maxPoint[i] = this->c[i] + this->r;
		}
    }
};
    
