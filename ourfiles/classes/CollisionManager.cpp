#include "CollisionManager.h"



CollisionManager::CollisionManager() {
	sphere_tree = new igl::AABB<Eigen::MatrixXd, 3>();
	special_sphere_tree = new igl::AABB<Eigen::MatrixXd, 3>();
	cube_tree = new igl::AABB<Eigen::MatrixXd, 3>();
}
CollisionManager::~CollisionManager() {
}
void CollisionManager::init(igl::opengl::glfw::Viewer* viewer) {
	viewer->load_mesh_from_file(*viewer->data_path + "/sphere.obj");
	sphere_tree = new igl::AABB<Eigen::MatrixXd, 3>();
	sphere_tree->init(viewer->data().V, viewer->data().F);
	viewer->data().MyTranslate(Eigen::Vector3f(0, -5, 0), true);

	viewer->load_mesh_from_file(*viewer->data_path + "/sphere.obj");
	viewer->data().MyScale(Eigen::Vector3f(2, 2, 2));
	special_sphere_tree = new igl::AABB<Eigen::MatrixXd, 3>();
	sphere_tree->init(viewer->data().V, viewer->data().F);
	viewer->data().MyTranslate(Eigen::Vector3f(0, -5, 0), true);

	viewer->load_mesh_from_file(*viewer->data_path + "/cube.obj");
	cube_tree = new igl::AABB<Eigen::MatrixXd, 3>();
	cube_tree->init(viewer->data().V, viewer->data().F);
	viewer->data().MyTranslate(Eigen::Vector3f(0, -5, 0), true);

	viewer->load_mesh_from_file(*viewer->data_path + "/bunny.off");
	viewer->data().MyScale(Eigen::Vector3f(10, 10, 10));
	bunny_tree = new igl::AABB<Eigen::MatrixXd, 3>();
	bunny_tree->init(viewer->data().V, viewer->data().F);
	viewer->data().MyTranslate(Eigen::Vector3f(0, -5, 0), true);

}
bool CollisionManager::detect_collision(igl::opengl::glfw::Viewer* viewer, int index, int type) {
	igl::AABB<Eigen::MatrixXd, 3>* first = sphere_tree;
	igl::AABB<Eigen::MatrixXd, 3>* second = sphere_tree;
	switch (type) {
	case 1:
		second = cube_tree;
		break;
	case 2:
		second = bunny_tree;
		break;
	case 3:
		second = special_sphere_tree;
	default:
		break;
	}
	return detect_collision(index, viewer, first, second);
}
bool CollisionManager::detect_collision(int index, igl::opengl::glfw::Viewer* viewer,
	igl::AABB<Eigen::MatrixXd, 3>* first,
	igl::AABB<Eigen::MatrixXd, 3>* second) {
	bool collision = false;

	//centers of boxes
	Eigen::Vector3d C1 = ((viewer->get_parent_link_T(viewer->last_link_index) * viewer->data_list[viewer->last_link_index].MakeTrans()).cast<double>() * Eigen::Vector4d(first->m_box.center()(0), first->m_box.center()(1), first->m_box.center()(2), 1)).block<3, 1>(0, 0);
	Eigen::Vector3d C2 = ((viewer->data_list[index].MakeTrans()).cast<double>() * Eigen::Vector4d(second->m_box.center()(0), second->m_box.center()(1), second->m_box.center()(2), 1)).block<3, 1>(0, 0);
	Eigen::Vector3d T = (C2 - C1);
	//axis of first box
	Eigen::Vector3d A[3] =
	{ (viewer->get_parent_link_Rot(viewer->last_link_index) * viewer->data_list[viewer->last_link_index].GetRotationTrans() * Eigen::Vector4f(1,0,0,1)).block<3,1>(0,0).cast<double>(),
		(viewer->get_parent_link_Rot(viewer->last_link_index) * viewer->data_list[viewer->last_link_index].GetRotationTrans() * Eigen::Vector4f(0,1,0,1)).block<3,1>(0,0).cast<double>(),
		(viewer->get_parent_link_Rot(viewer->last_link_index) * viewer->data_list[viewer->last_link_index].GetRotationTrans() * Eigen::Vector4f(0,0,1,1)).block<3,1>(0,0).cast<double>() };
	//axis of second box
	Eigen::Vector3d B[3] =
	{ (viewer->data_list[index].GetRotationTrans() * Eigen::Vector4f(1,0,0,1)).block<3,1>(0,0).cast<double>(),
		(viewer->data_list[index].GetRotationTrans() * Eigen::Vector4f(0,1,0,1)).block<3,1>(0,0).cast<double>(),
		(viewer->data_list[index].GetRotationTrans() * Eigen::Vector4f(0,0,1,1)).block<3,1>(0,0).cast<double>() };
	//dimentions of boxes
	Eigen::Vector3d dims[2] = { first->m_box.sizes().cast<double>() / 2  , second->m_box.sizes().cast<double>() / 2 };
	igl::AABB<Eigen::MatrixXd, 3>* first_left = first->m_left;
	igl::AABB<Eigen::MatrixXd, 3>* first_right = first->m_right;
	igl::AABB<Eigen::MatrixXd, 3>* second_left = second->m_left;
	igl::AABB<Eigen::MatrixXd, 3>* second_right = second->m_right;
	if (first == nullptr || second == nullptr) return collision;
	//------------------------------------------
	//while we're not checking a leaf
	if (!(first->is_leaf() || second->is_leaf())) {
		if (!can_seperate(index, viewer, first->m_box, second->m_box, T, A, B, dims)) {
			//-------------------------------------------
			//if we couldn't seperate parent box we have to check children
			//start checking collisions between children
			if (!collision && !can_seperate(index, viewer, first_right->m_box, second_left->m_box, T, A, B, dims)) {
				collision = detect_collision(index, viewer, first_right, second_left);
			}
			if (!collision && !can_seperate(index, viewer, first_right->m_box, second_right->m_box, T, A, B, dims)) {
				collision = detect_collision(index, viewer, first_right, second_right);
			}
			if (!collision && !can_seperate(index, viewer, first_left->m_box, second_left->m_box, T, A, B, dims)) {
				collision = detect_collision(index, viewer, first_left, second_left);
			}
			if (!collision && !can_seperate(index, viewer, first_left->m_box, second_right->m_box, T, A, B, dims)) {
				collision = detect_collision(index, viewer, first_left, second_right);
			}
		}
	}
	else if (first->is_leaf() && second->is_leaf()) {
		collision = !can_seperate(index, viewer, first->m_box, second->m_box, T, A, B, dims);
	}
	else if (first->is_leaf() && !second->is_leaf()) {
		if (!collision && !can_seperate(index, viewer, first->m_box, second_left->m_box, T, A, B, dims)) {
			collision = detect_collision(index, viewer, first, second_left);
		}
		if (!collision && !can_seperate(index, viewer, first->m_box, second_right->m_box, T, A, B, dims)) {
			collision = detect_collision(index, viewer, first, second_right);
		}
	}
	else if (!first->is_leaf() && second->is_leaf()) {
		if (!collision && !can_seperate(index, viewer, first_left->m_box, second->m_box, T, A, B, dims)) {
			collision = detect_collision(index, viewer, first_left, second);
		}
		if (!collision && !can_seperate(index, viewer, first_right->m_box, second->m_box, T, A, B, dims)) {
			collision = detect_collision(index, viewer, first_right, second);
		}
	}
	return collision;
}
bool CollisionManager::can_seperate(int object_index, igl::opengl::glfw::Viewer* viewer,
	Eigen::AlignedBox<double, 3>& first,
	Eigen::AlignedBox<double, 3>& second,
	Eigen::Vector3d& T,
	Eigen::Vector3d* A,
	Eigen::Vector3d* B,
	Eigen::Vector3d* dims
) {
	for (int dim = 0; dim < 3; dim++) {
		double left_side = (dims[0](dim) + std::abs(dims[1](0) * (A[dim].dot(B[0]))) + std::abs(dims[1](1) * (A[dim].dot(B[1]))) + std::abs(dims[1](2) * (A[dim].dot(B[2]))));
		double right_side = std::abs(T.dot(A[dim]));
		if (right_side > left_side) return true;
	}
	for (int dim = 0; dim < 3; dim++) {
		double left_side = (dims[1](dim) + std::abs(dims[0](0) * (A[0].dot(B[dim]))) + std::abs(dims[0](1) * (A[1].dot(B[dim]))) + std::abs(dims[0](2) * (A[2].dot(B[dim]))));
		double right_side = std::abs(T.dot(B[dim]));
		if (right_side > left_side)return true;
	}
	//case 7
	double left_side = std::abs(dims[0](1) * A[2].dot(B[0])) + std::abs(dims[0](2) * A[1].dot(B[0])) + std::abs(dims[1](1) * A[0].dot(B[2])) + std::abs(dims[1](2) * A[0].dot(B[1]));
	double right_side = std::abs((T.dot(A[2])) * (A[1].dot(B[0])) - (T.dot(A[1])) * (A[2].dot(B[0])));
	if (right_side > left_side) return true;

	//case 8
	left_side = std::abs(dims[0](1) * A[2].dot(B[1])) + std::abs(dims[0](2) * A[1].dot(B[1])) + std::abs(dims[1](0) * A[0].dot(B[2])) + std::abs(dims[1](2) * A[0].dot(B[0]));
	right_side = std::abs((T.dot(A[2])) * (A[1].dot(B[1])) - (T.dot(A[1])) * (A[2].dot(B[1])));
	if (right_side > left_side)return true;
	//case 9
	left_side = std::abs(dims[0](1) * A[2].dot(B[2])) + std::abs(dims[0](2) * A[1].dot(B[2])) + std::abs(dims[1](0) * A[0].dot(B[1])) + std::abs(dims[1](1) * A[0].dot(B[0]));
	right_side = std::abs((T.dot(A[2])) * (A[1].dot(B[2])) - (T.dot(A[1])) * (A[2].dot(B[2])));
	if (right_side > left_side) return true;
	//case 10
	left_side = std::abs(dims[0](0) * A[2].dot(B[0])) + std::abs(dims[0](2) * A[0].dot(B[0])) + std::abs(dims[1](1) * A[1].dot(B[2])) + std::abs(dims[1](2) * A[1].dot(B[1]));
	right_side = std::abs((T.dot(A[0])) * (A[2].dot(B[0])) - (T.dot(A[2])) * (A[0].dot(B[0])));
	if (right_side > left_side) return true;
	//case 11
	left_side = std::abs(dims[0](0) * A[2].dot(B[1])) + std::abs(dims[0](2) * A[0].dot(B[1])) + std::abs(dims[1](0) * A[1].dot(B[2])) + std::abs(dims[1](2) * A[1].dot(B[0]));
	right_side = std::abs((T.dot(A[0])) * (A[2].dot(B[1])) - (T.dot(A[2])) * (A[0].dot(B[1])));
	if (right_side > left_side) return true;
	//case 12
	left_side = std::abs(dims[0](0) * A[2].dot(B[2])) + std::abs(dims[0](2) * A[0].dot(B[2])) + std::abs(dims[1](0) * A[1].dot(B[1])) + std::abs(dims[1](1) * A[1].dot(B[0]));
	right_side = std::abs((T.dot(A[0])) * (A[2].dot(B[2])) - (T.dot(A[2])) * (A[0].dot(B[2])));
	if (right_side > left_side)return true;
	//case 13
	left_side = std::abs(dims[0](0) * A[1].dot(B[0])) + std::abs(dims[0](1) * A[0].dot(B[0])) + std::abs(dims[1](1) * A[2].dot(B[2])) + std::abs(dims[1](2) * A[2].dot(B[1]));
	right_side = std::abs((T.dot(A[1])) * (A[0].dot(B[0])) - (T.dot(A[0])) * (A[1].dot(B[0])));
	if (right_side > left_side) return true;
	//case 14
	left_side = std::abs(dims[0](0) * A[1].dot(B[1])) + std::abs(dims[0](1) * A[0].dot(B[1])) + std::abs(dims[1](0) * A[2].dot(B[2])) + std::abs(dims[1](2) * A[2].dot(B[0]));
	right_side = std::abs((T.dot(A[1])) * (A[0].dot(B[1])) - (T.dot(A[0])) * (A[1].dot(B[1])));
	if (right_side > left_side)return true;
	//case 15
	left_side = std::abs(dims[0](0) * A[1].dot(B[2])) + std::abs(dims[0](1) * A[0].dot(B[2])) + std::abs(dims[1](0) * A[2].dot(B[1])) + std::abs(dims[1](1) * A[2].dot(B[0]));
	right_side = std::abs((T.dot(A[1])) * (A[0].dot(B[2])) - (T.dot(A[0])) * (A[1].dot(B[2])));
	if (right_side > left_side)return true;

	return false;
}
