#pragma once

#include "math3d.h"

template<typename T>
void setMinMax(vec3& min, vec3& max, T object)
{
	vec3 objMin = object.minPoint;
	vec3 objMax = object.maxPoint;
	for (int i = 0; i < 3; i++)
	{
		if (objMax[i] > max[i]) max[i] = objMax[i];			//Set the maxpoint[n] to point[n] if point[n] is greater
		else if (objMin[i] < min[i]) min[i] = objMin[i];	//Do the same for the min
	}
}