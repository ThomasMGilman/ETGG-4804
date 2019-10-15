#pragma once

#define MAXDEPTH 3
#define MAXTRIS 12

#include <array>
#include "Scene.h"

using namespace std;


float rayPlaneIntersection(const vec3& N, float D, vec3& s, vec3& v)
{
	float denom = dot(N, v);
	//if denom is zero, we get t=infinity
	float numer = -(D + dot(N, s));
	float t = numer / denom;
	return t;
}

void planePairIntersection(const vec4& p1, const vec4& p2, vec3&
	s, vec3& v, float& t1, float& t2) {
	t1 = rayPlaneIntersection(p1.xyz(), p1.w, s, v);
	t2 = rayPlaneIntersection(p2.xyz(), p2.w, s, v);
	if (t1 > t2) {
		float tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
}

/*BoxTest*/
//planes 0,1 = x min/max planes 
//planes 2,3 = y min/max
//planes 4,5 = z min/max
bool rayBoxIntersection(array<vec4, 6> & planes, vec3& s, vec3& v) {
	using namespace std;
	float tx1, tx2;
	planePairIntersection(planes[0], planes[1], s, v, tx1, tx2);
	float ty1, ty2;
	planePairIntersection(planes[2], planes[3], s, v, ty1, ty2);
	float tz1, tz2;
	planePairIntersection(planes[4], planes[5], s, v, tz1, tz2);
	float tmin = max(tx1, max(ty1, tz1));
	float tmax = min(tx2, min(ty2, tz2));
	return tmin <= tmax;
}

class OctreeNode {
public:
	vec3 min, max;
	std::array<unsigned, 8> children;
	std::vector<Triangle> triangles;	//only used for leaf
	std::array<vec4, 6> planes;			//six planes of this node
	static vector<OctreeNode> nodes;	//all nodes

	OctreeNode(Mesh& sceneMesh, int depth)
	{

	}

	void initialize(int depth, vector<Triangle>& tris) {
		if (depth >= MAXDEPTH || tris.size() <= MAXTRIS) {
			this->triangles = tris;
			this->children = { 0 };		//set all children to 0
			
		}
		else {
			int idx = nodes.size();
			//add 8 child to nodes[]
			vector<Triangle> TV[8];
			for (Triangle& T : tris) {
				for (int j = 0; j < 8; ++j) {
					if (nodes[idx + j].contains(T))
						TV[j].push_back(T);
				}
			}
			for (int j = 0; j < 8; ++j)
				nodes[idx + j].initialize(depth+1 ,TV[j]);
		}
	}

	bool contains(Triangle& T)
	{
		for (int i = 0; i < 3; ++i) {
			if (pointInBox(T.p[i], this))
				return true;
		}
		for (int i = 0; i < 3; ++i) {
			if (segmentBoxIntersection(T.p[i], T.p[(i + 1) % 3], this))
				return true;
		}
		for (int p = 0; p < this->planes.size(); p++)
		{
		}
		for (all twelve edges e of this) {
			if (segmentTriangleIntersection(e, T))
				return true;
		}
		return false;
	}

	bool pointInBox(vec3 point, OctreeNode* node)
	{

	}

	bool segmentBoxIntersection(vec3 p1, vec3 p2, OctreeNode* node)
	{

	}

	bool segmentTriangleIntersection(edge ,Triangle& T)
	{

	}
};

