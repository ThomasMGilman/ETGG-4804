//Thomas Gilman
//Jim Hudson
//ETGG 4804_1
//Optomization Techniques
//3rd September, 2019
#pragma once

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

		//q = rayStart - sphereCenter
		//q * q - r^2 + 2(q * v)t + t^2(v * v) = 0
		vec3 q = s - scene.spheres[i].c;
		float radius = scene.spheres[i].r;
		float A = dot(v, v);
		float B = 2 * dot(q, v);
		float C = dot(q, q) - (radius * radius);
		float discriminant = (B * B) - (4 * A * C);

		if (discriminant >= 0)	//if the discriminant is less than zero, there is no intersection
		{
			float sqrtDiscrim = sqrt(discriminant);
			float denom = 2 * A;

			//use Quadratic Formula to check if a spheres t's are closer and update closestT if they are
			float t1 = (-B + sqrtDiscrim) / denom;
			float t2 = (-B - sqrtDiscrim) / denom;

			if (t1 < 0 && t2 >= 0)			//t2 is closer than ClosestT
			{
				if (closestT > t2)
				{
					closestT = t2;
					ci = i;
				}
			}
			else if (t1 >= 0 && t2 < 0)		//t1 is closer than ClosestT
			{
				if (closestT > t1)
				{
					closestT = t1;
					ci = i;
				}
			}
			else if (t1 >= 0 && t2 >= 0)	//one of the t's is closer than ClosestT
			{
				float minT = fminf(t1, t2);
				if (closestT > minT)
				{
					closestT = minT;
					ci = i;
				}
			}
		}
    }
    
    if( ci == (unsigned)-1 ){
		if (!inter)			//already true for another object
			inter = false;
        return;
    }
	else
	{
		ip = s + closestT * v;						//intersection point
		N = normalize(ip - scene.spheres[ci].c);	//Normal at intersection point
		color = scene.spheres[ci].color;
		inter = true;
	}
}

void traceTriangles(Scene& scene, vec3& s, vec3& v, vec3& ip, vec3& N, 
            vec3& color, float& closestT, bool& inter)
{
	unsigned ti = (unsigned)-1; //triangle intersection
	Mesh *cm = nullptr;			//closest mesh to draw pointer
	//check meshes for closest triangle
    for(auto& M : scene.meshes ){
		
        for(unsigned i=0;i<M.triangles.size();++i){
            Triangle& T = M.triangles[i];
			vec3 p = T.p[0];
			vec3 q = T.p[1];
			vec3 r = T.p[2];

			vec3 N = cross((q-r), (r-p));	//Triangles Normal
			float D = -dot(N, p);			//Distance of plane from origin
			float denominator = dot(N, v);
			
			//does the ray intersect the triangle
			//No intersection if denominator is zero
			if (denominator != 0)				
			{
				float t = -(D + dot(N, s)) / denominator;
				if (t >= 0)
				{
					//std::cout << "got intersection: " << t << std::endl;
					if (closestT > t)
					{
						vec3 currentIP = s + (t*v); //intersection point

						float sumQi = 0;
						for (int i = 0; i < T.e->len(); i++)	//make sure ip is intersecting the triangle
						{
							vec3 qi = cross(T.e[i], (currentIP - T.p[i]));
							sumQi += qi.magnitude();
						}

						float A = sumQi * T.oneOverTwiceArea;
						if (A <= 1.001)
						{
							ip = currentIP;
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
		if(!inter)			//already true for another object
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
