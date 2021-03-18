#include "menger.h"
#include "iostream"
#include <glm/gtx/string_cast.hpp>

using namespace std;

namespace {
	const int kMinLevel = 0;
	const int kMaxLevel = 4;
};

Menger::Menger()
{
	// Add additional initialization if you like
}

Menger::~Menger()
{
}

void
Menger::set_nesting_level(int level)
{
	nesting_level_ = level;
	dirty_ = true;
}

bool
Menger::is_dirty() const
{
	return dirty_;
}

void
Menger::set_clean()
{
	dirty_ = false;
}

// FIXME generate Menger sponge geometry
void
Menger::generate_geometry(std::vector<glm::vec4>& obj_vertices,
                          std::vector<glm::uvec3>& obj_faces) const
{
	glm::vec3 M = glm::vec3(.5,.5,.5);
	glm::vec3 m = glm::vec3(-1*M.x, -1*M.y, -1*M.z);
	// Create starting cube
	obj_vertices.push_back(glm::vec4(m.x,m.y,m.z-2, 1.0f));	// 0, far-bot left
	obj_vertices.push_back(glm::vec4(M.x,m.y,m.z-2, 1.0f));	// 1, far-bot right
	obj_vertices.push_back(glm::vec4(M.x,m.y,M.z-2, 1.0f));	// 2, near-bot right
	obj_vertices.push_back(glm::vec4(m.x,m.y,M.z-2, 1.0f));	// 3, near_bot left
	obj_vertices.push_back(glm::vec4(m.x,M.y,m.z-2, 1.0f));	// 4, far-top left
	obj_vertices.push_back(glm::vec4(M.x,M.y,m.z-2, 1.0f));	// 5, far-top right
	obj_vertices.push_back(glm::vec4(M.x,M.y,M.z-2, 1.0f));	// 6, near-top right
	obj_vertices.push_back(glm::vec4(m.x,M.y,M.z-2, 1.0f));	// 7, near-top left
	
	// Triangulate faces
	// first pass
	obj_faces.push_back(glm::uvec3(0, 4, 5));
	obj_faces.push_back(glm::uvec3(1, 5, 6));
	obj_faces.push_back(glm::uvec3(2, 6, 7));
	obj_faces.push_back(glm::uvec3(3, 7, 4));
	obj_faces.push_back(glm::uvec3(3, 0, 1));
	obj_faces.push_back(glm::uvec3(4, 7, 6));
	// second pass
	obj_faces.push_back(glm::uvec3(6, 5, 4));
	obj_faces.push_back(glm::uvec3(1, 2, 3));
	obj_faces.push_back(glm::uvec3(4, 0, 3));
	obj_faces.push_back(glm::uvec3(7, 3, 2));
	obj_faces.push_back(glm::uvec3(6, 2, 1));
	obj_faces.push_back(glm::uvec3(5, 1, 0));


	Menger::subdivide(nesting_level_, M, m, obj_vertices, obj_faces);

	// cout << "test\n";
	//draw cube here
	//CreateMenger()
}

void
Menger::CreateMenger(glm::vec3 M, glm::vec3 m, std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces) const
{
	int offset = obj_vertices.size();
	// Create starting cube
	obj_vertices.push_back(glm::vec4(m.x,m.y,m.z-2, 1.0f));	// 0, far-bot left
	obj_vertices.push_back(glm::vec4(M.x,m.y,m.z-2, 1.0f));	// 1, far-bot right
	obj_vertices.push_back(glm::vec4(M.x,m.y,M.z-2, 1.0f));	// 2, near-bot right
	obj_vertices.push_back(glm::vec4(m.x,m.y,M.z-2, 1.0f));	// 3, near_bot left
	obj_vertices.push_back(glm::vec4(m.x,M.y,m.z-2, 1.0f));	// 4, far-top left
	obj_vertices.push_back(glm::vec4(M.x,M.y,m.z-2, 1.0f));	// 5, far-top right
	obj_vertices.push_back(glm::vec4(M.x,M.y,M.z-2, 1.0f));	// 6, near-top right
	obj_vertices.push_back(glm::vec4(m.x,M.y,M.z-2, 1.0f));	// 7, near-top left
	
	// Triangulate faces
	// first pass
	obj_faces.push_back(glm::uvec3(0+offset, 4+offset, 5+offset));
	obj_faces.push_back(glm::uvec3(1+offset, 5+offset, 6+offset));
	obj_faces.push_back(glm::uvec3(2+offset, 6+offset, 7+offset));
	obj_faces.push_back(glm::uvec3(3+offset, 7+offset, 4+offset));
	obj_faces.push_back(glm::uvec3(3+offset, 0+offset, 1+offset));
	obj_faces.push_back(glm::uvec3(4+offset, 7+offset, 6+offset));
	// second pass
	obj_faces.push_back(glm::uvec3(6+offset, 5+offset, 4+offset));
	obj_faces.push_back(glm::uvec3(1+offset, 2+offset, 3+offset));
	obj_faces.push_back(glm::uvec3(4+offset, 0+offset, 3+offset));
	obj_faces.push_back(glm::uvec3(7+offset, 3+offset, 2+offset));
	obj_faces.push_back(glm::uvec3(6+offset, 2+offset, 1+offset));
	obj_faces.push_back(glm::uvec3(5+offset, 1+offset, 0+offset));
}

void 
Menger::subdivide(int depth, glm::vec3 M, glm::vec3 m, std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces) const{
	cout << "depth " << depth << endl;
	if(depth == 0){
		Menger::CreateMenger(M, m, obj_vertices, obj_faces);
		return;
	}
	double L = pow(1.0/3.0, depth);
	for(int k=0; k<3; k++) {				// bottom to top layers
		for(int j=0; j<3; j++) {			// back to front cubes
			for(int l=0; l<3; l++) {		// left to right cubes
				int offset = k*j*l;
				if(offset != 4 || offset != 10 || offset != 12 || offset != 13 || offset != 14 || offset != 16 || offset != 22) {
					glm::dvec3 M_kj;		// max bounds
					glm::dvec3 m_kj;		// min bounds
					if(j==0){
						if(l==0) {
							M_kj = glm::vec3(M.x-L, M.y-L, M.z-L);
							m_kj = glm::vec3(m.x-L, m.y-L, m.z-L);
							subdivide(depth-1, M_kj, m_kj, obj_vertices, obj_faces);
						}
						if(l==1) {
							M_kj = glm::vec3(M.x-L, M.y-L, M.z-L);
							m_kj = glm::vec3(m.x-L, m.y-L, m.z-L);
							subdivide(depth-1, M_kj, m_kj, obj_vertices, obj_faces);
						}
						if(l==2) {

						}
					}
					if(j==0){
						if(l==0) {

						}
						if(l==1) {

						}
						if(l==2) {

						}
					}
					if(j==0){
						if(l==0) {

						}
						if(l==1) {

						}
						if(l==2) {

						}
					}
					//CreateMenger(M_kj, m_kj, offset, obj_vertices, obj_faces);
				}
			}
		}
	}
}
