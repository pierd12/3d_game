#pragma once
#include <vector>
#include <igl\AABB.h>
#include <igl\opengl\glfw\Viewer.h>

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
	void init(igl::opengl::glfw::Viewer* viewer);
	bool detect_collision(igl::opengl::glfw::Viewer* viewer, int index, int type);
private:
	int objects;
	int types;
	igl::AABB<Eigen::MatrixXd, 3>* sphere_tree;
	igl::AABB<Eigen::MatrixXd, 3>* special_sphere_tree;
	igl::AABB<Eigen::MatrixXd, 3>* cube_tree;
	igl::AABB<Eigen::MatrixXd, 3>* bunny_tree;
	std::vector< Eigen::AlignedBox<double, 3>*> object_boxes;
	bool can_seperate(int object_index, igl::opengl::glfw::Viewer* viewer,
		Eigen::AlignedBox<double, 3>& first,
		Eigen::AlignedBox<double, 3>& second,
		Eigen::Vector3d& T,
		Eigen::Vector3d* A,
		Eigen::Vector3d* B,
		Eigen::Vector3d* dims);
	bool detect_collision(int index, igl::opengl::glfw::Viewer* viewer, igl::AABB<Eigen::MatrixXd, 3>* first, igl::AABB<Eigen::MatrixXd, 3>* second);
};