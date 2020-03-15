#include "Snake.h"
#include <iostream>
#include <igl\opengl\ViewerCore.h>



Snake::Snake(int length, igl::opengl::glfw::Viewer* viewer) {
	this->length = length;
	this->viewer = viewer;
}

Snake::~Snake() {
	std::cout << "i'm dying :'(" << std::endl;
}

void Snake::init() {
	//init body
	int link;
	for (link = 0; link < this->length; link++) {
		viewer->load_mesh_from_file(*viewer->data_path + "/ycylinder.obj");
		if (link == 0) {
			viewer->root_link_index = viewer->selected_data_index;
			head_index = viewer->selected_data_index;
		}
		else viewer->data().MyTranslate(Eigen::Vector3f(0, 1.6, 0), true);
		viewer->data().show_lines = false;
		viewer->data().SetCenterOfRotation(Eigen::Vector3f(0, -0.8, 0));
		
	}
	//init head
	viewer->load_mesh_from_file(*viewer->data_path + "/sphere.obj");
	viewer->last_link_index = viewer->selected_data_index;
	viewer->data().MyTranslate(Eigen::Vector3f(0, 1.6, 0), true);
	viewer->data().SetCenterOfRotation(Eigen::Vector3f(0, -0.8, 0));
	viewer->data().show_lines = false;
	head_index = viewer->selected_data_index;
	initial_tip_position = get_tip_position();
	
}

void Snake::set_camera_on_head() {
	Eigen::Vector3f head = get_tip_position();
	Eigen::Vector3f eye = (viewer->MakeTrans()*
			Eigen::Vector4f(head(0), head(1), head(2), 1)).block<3, 1>(0, 0);
	//std::cout << head << std::endl;
	//std::cout << head << std::endl;   0		18.4 		0
	//std::cout << eye << std::endl;		0		8.4  - 30
	//viewer->second_camera->camera_eye = Eigen::Vector3f(0, -5, 30);
	
	viewer->second_camera->camera_up = Eigen::Vector3f(0, 15,0);;
	
	//viewer->second_camera->camera_translation = Eigen::Vector3f(0, 0, 100);
	
	viewer->second_camera->camera_eye = Eigen::Vector3f(eye(0), eye(1), -(eye(2)));
	
	//viewer->second_camera->camera_up = Eigen::Vector3f(head(0), head(1), head(2));;
	//std::cout << viewer->second_camera->camera_up << std::endl;
	//eye(2));
	//viewer->second_camera->camera_translation = Eigen::Vector3f(0,-10,100);
	//viewer->second_camera->camera_zoom += 0.8;
	
	viewer->second_camera->camera_translation = Eigen::Vector3f(2.2*eye(0), 2.45*eye(1), -(eye(2)*4.3));

}

void Snake::speak() {
	std::cout << "sssssssss i ssssss will sssssssss kill ssssssss you!!!!ssss!!!!" << std::endl;
}

void Snake::dance() {
	
	//curve.show(viewer->data());
	for (auto& mesh : viewer->data_list)
	{
		

	}
}

void Snake::move_to(Eigen::Vector3f& position) {
	for (int i = viewer->root_link_index + length - 1; i > viewer->root_link_index; i--) {
		Eigen::Vector3f R = (viewer->get_parent_link_T(i) * viewer->data_list[i].MakeTrans() * Eigen::Vector4f(0, -0.8, 0, 1)).block<3, 1>(0, 0);
		Eigen::Vector3f E = (viewer->get_parent_link_T(viewer->last_link_index) * viewer->data_list[viewer->last_link_index].MakeTrans() * Eigen::Vector4f(0, 0.8, 0, 1)).block<3, 1>(0, 0);
		Eigen::Vector3f RD = position - R;
		Eigen::Vector3f RE = E - R;
		auto angle1 = acosf(RD.normalized().dot(RE.normalized())) - 0.03;
		Eigen::Vector3f axle_of_rotation = (viewer->get_parent_link_T(i).inverse()).block<3, 3>(0, 0) * RD.cross(RE);
		viewer->data_list[i].MyRotate(axle_of_rotation, -angle1 * 0.1);
	}
	
	Eigen::Vector3f R = (viewer->data_list[viewer->root_link_index].MakeTrans() * Eigen::Vector4f(0, -0.8, 0, 1)).block<3, 1>(0, 0);
	Eigen::Vector3f E = (viewer->get_parent_link_T(viewer->last_link_index) * viewer->data_list[viewer->last_link_index].MakeTrans() * Eigen::Vector4f(0, 0.8, 0, 1)).block<3, 1>(0, 0);
	Eigen::Vector3f RD = position - R;
	Eigen::Vector3f RE = E - R;
	auto angle1 = acosf(RD.normalized().dot(RE.normalized())) - 0.03;
	Eigen::Vector3f axle_of_rotation = RD.cross(RE);
	viewer->data_list[viewer->root_link_index].MyRotate(axle_of_rotation, -angle1 * 0.1);
	set_camera_on_head();
}

bool Snake::can_reach(Eigen::Vector3f& position) {
	Eigen::Vector3f s(0, -0.8, 0);
	if (length * 1.6 - (position - s).norm() > 0) {
		return true;
	}
	else return false;
}

Eigen::Vector3f Snake::get_tip_position() {
	Eigen::Vector3f tip = (
		viewer->get_parent_link_T(viewer->last_link_index)
		* viewer->data_list[viewer->last_link_index].MakeTrans()
		* Eigen::Vector4f(0, 0.8, 0, 1)).block<3, 1>(0, 0).transpose();
	return tip;
}

bool Snake::reached(Eigen::Vector3f& position) {
	Eigen::Vector3f tip = get_tip_position();
	if ((position - tip).norm() < 2) return true;
	else return false;
}

bool Snake::is_snake_selected(int index) {
	if (index >= head_index && index <= last_index) return true;
	return false;
}