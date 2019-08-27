#pragma once

//work on this
//s : ray start
//v : ray direction
//N : normal
//ip : intersection point
void traceSpheres(Scene& scene, vec3& s, vec3& v, vec3& ip, vec3& N, vec3& color, float& closestT, bool& inter)
{
    unsigned ci=(unsigned)-1;

	//go through the list of spheres in the scene 
	//and determine if the viewer is intersecting the object
    for(unsigned i=0;i<scene.spheres.size();++i){
		vec3 q = s - scene.spheres[i].c;

		float radius = scene.spheres[i].r;
		float A = dot(v, v);
		float B = 2 * dot(q, v);
		float C = dot(q, q) - (radius * radius);

		//use Quadratic Formula to check if a spheres t is closer and update closestT
		float t1 = (-B + sqrt((B * B) - (4 * A * C))) / (2 * A);
		float t2 = (-B - sqrt((B * B) - (4 * A * C))) / (2 * A);

		if (t1 < 0 && t2 >= 0)
		{
			if (closestT > t2)
			{
				closestT = t2;
				ci = i;
			}
		}
		else if (t1 >= 0 && t2 < 0)
		{
			if (closestT > t1)
			{
				closestT = t1;
				ci = i;
			}
		}
		else if (t1 >= 0 && t2 >= 0)
		{
			float minT = fminf(t1, t2);
			if (closestT > minT)
			{
				closestT = minT;
				ci = i;
			}
		}
    }
    
    if( ci == (unsigned)-1 ){
        inter=false;
        return;
    }
    ip = s + closestT * v;
    N = normalize(ip - scene.spheres[ci].c);
    color = scene.spheres[ci].color;
    inter=true;
}

//work on this too
void traceTriangles(Scene& scene, vec3& s, vec3& v, vec3& ip, vec3& N, 
            vec3& color, float& closestT, bool& inter)
{
	unsigned ti = (unsigned)-1; //triangle intersection
	Mesh* cm;					//closest mesh to draw

    for(auto& M : scene.meshes ){
		
        for(unsigned i=0;i<M.triangles.size();++i){
            Triangle& T = M.triangles[i];
			float denom = dot(T.N, v);
			
			//does the ray intersect the triangle
			if (denom != 0)				
			{
				float t = (-(T.D + (dot(T.N, s)))) / denom;
				if (t >= 0)
				{
					if (closestT > t)
					{
						closestT = t;
						ti = i;
						*cm = M;
					}
				}
			}
        }
		
		if (ti == (unsigned)-1) {	//no intersection
			inter = false;
			return;
		}

		Triangle& T = cm->triangles[ti];	//get triangle to draw
		ip = s + (closestT * v);
		//continue here
    }
}

bool traceRay(Scene& scene, vec3& s, vec3& v, vec3& ip, vec3& N, vec3& color)
{
    float closestT = 1E99;
    bool inter;
    traceSpheres(scene,s,v,ip,N,color,closestT,inter);
    traceTriangles(scene,s,v,ip,N,color,closestT,inter);
    return inter;
}