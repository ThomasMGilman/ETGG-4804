#pragma once

//work on this
//s : ray start
//v : ray direction
//N : normal
//ip : intersection point
void traceSpheres(Scene& scene, vec3& s, vec3& v, vec3& ip, vec3& N, vec3& color, float& closestT, bool& inter)
{
	//std::cout << "sphere" << std::endl;
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
	//std::cout << "triangle" << std::endl;
	unsigned ti = (unsigned)-1; //triangle intersection
	Mesh *cm = nullptr;			//closest mesh to draw pointer

	//check meshes for closest triangle
    for(auto& M : scene.meshes ){
		
        for(unsigned i=0;i<M.triangles.size();++i){
            Triangle& T = M.triangles[i];
			float denominator = dot(T.N, v);
			
			//does the ray intersect the triangle
			if (denominator != 0)				
			{
				float t = (-(T.D + (dot(T.N, s)))) / denominator;
				if (t >= 0)
				{
					//std::cout << "got intersection: " << t << std::endl;
					if (closestT > t)
					{
						//std::cout << "got closer intersection: " << t <<std::endl;
						float sumQi = 0;
						for (int i = 0; i < T.e->len(); i++)	//make sure ip is intersecting the triangle
						{
							vec3 qi = cross(T.e[i], (ip - T.p[i]));
							sumQi += qi.magnitude();
						}

						float A = sumQi * T.oneOverTwiceArea;
						if (A <= 1.001)
						{
							closestT = t;
							ti = i;
							cm = &M;
						}
					}
				}
			}
        }
    }

	if ( cm == nullptr || ti == (unsigned)-1) {	//no intersection

		//std::cout << "no intersection, or mesh is null Mesh: " << std::endl;
		inter = false;
		return;
	}
	else
	{
		Triangle& T = cm->triangles[ti];		//get triangle to draw from closest mesh
		N = T.N;
		color = cm->color;
		inter = true;
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
