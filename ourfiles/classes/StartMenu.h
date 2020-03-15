#pragma once
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiHelpers.h>
#include <imgui/imgui.h>
#include <iostream>
#include <GLFW/glfw3.h>

class StartMenu
{
public:
	StartMenu(igl::opengl::glfw::Viewer* viewer, 
			igl::opengl::glfw::imgui::ImGuiMenu* menu);
	~StartMenu();
	void init();
	void main_menu();
	void start_menu();
	void pause_menu();
	void finish_menu();
	void game_menu();
	void game_info();
	void test();
	bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);

private:
	bool viewer_info = false;
	char name[128] = "";
	igl::opengl::glfw::Viewer* viewer;
	igl::opengl::glfw::imgui::ImGuiMenu* menu;
};

