#pragma once
#include <igl\opengl\glfw\Viewer.h>

class Snake
{
public:
	Snake(int length, igl::opengl::glfw::Viewer* viewer);
	~Snake();
	void init();
	void set_camera_on_head();
	void move_to(Eigen::Vector3f& position);
	bool can_reach(Eigen::Vector3f& position);
	bool reached(Eigen::Vector3f& position);
	Eigen::Vector3f get_tip_position();
	bool is_snake_selected(int index);
	void speak();
	void dance();
	Eigen::Vector3f initial_tip_position;

private:
	int length;
	int head_index;
	int last_index;
	igl::opengl::glfw::Viewer* viewer;
};

