#include "SurroundingManager.h"
#include <igl/png/readPNG.h>
SurroundingManager::SurroundingManager(igl::opengl::glfw::Viewer* viewer) {
	this->viewer = viewer;
	igl::png::readPNG("img/grass.png", Rg, Gg, Bg, Ag);
	igl::png::readPNG("img/sand.png", Rs, Gs, Bs, As);
	igl::png::readPNG("img/sky.png", Rsk, Gsk, Bsk, Ask);
	igl::png::readPNG("img/mountain.png", Rm, Gm, Bm, Am);
	viewer->load_mesh_from_file(*viewer->data_path + "/cube.obj");
	viewer->data().MyScale(Eigen::Vector3f(100, 0.1, 100));
	viewer->data().MyTranslate(Eigen::Vector3f(0, -1, 0), true);
	ground = viewer->selected_data_index;
	/*viewer->load_mesh_from_file(*viewer->data_path + "/cube.obj");
	viewer->data().MyScale(Eigen::Vector3f(20, 100, 100));
	viewer->data().MyTranslate(Eigen::Vector3f(100, 0, 0), true);
	sides = viewer->selected_data_index;
	viewer->load_mesh_from_file(*viewer->data_path + "/cube.obj");
	viewer->data().MyScale(Eigen::Vector3f(500, 500, 10));
	viewer->data().MyTranslate(Eigen::Vector3f(0, 40, -120), true);
	sky = viewer->selected_data_index;*/

	///speacial event
	//right
	viewer->load_mesh_from_file(*viewer->data_path + "/cube.obj");
	viewer->data().MyTranslate(Eigen::Vector3f(400, 0, 0), true);
	viewer->data().MyScale(Eigen::Vector3f(40, 40, 80));
	surr_index_special = viewer->selected_data_index;
	///front
	viewer->load_mesh_from_file(*viewer->data_path + "/cube.obj");
	viewer->data().MyTranslate(Eigen::Vector3f(400, 0, 0), true);
	viewer->data().MyScale(Eigen::Vector3f(40, 40, 20));
	///left
	viewer->load_mesh_from_file(*viewer->data_path + "/cube.obj");
	viewer->data().MyTranslate(Eigen::Vector3f(400, 0, 0), true);
	viewer->data().MyScale(Eigen::Vector3f(40, 40, 80));
	//up
	viewer->load_mesh_from_file(*viewer->data_path + "/cube.obj");
	viewer->data().MyTranslate(Eigen::Vector3f(400, 0, 0), true);
	viewer->data().MyScale(Eigen::Vector3f(40, 40, 80));
	
}
SurroundingManager::~SurroundingManager(){}

void SurroundingManager::deleteSurr() {
	viewer->data(sides).clear();
	viewer->data(ground).clear();
}

void SurroundingManager::set_grass() {
	viewer->data(ground).show_texture = true;
	viewer->data(ground).set_texture(Rg, Gg, Bg);
}
void SurroundingManager::set_sand() {
	viewer->data(ground).show_texture = true;
	viewer->data(ground).set_texture(Rs, Gs, Bs);
}
void SurroundingManager::set_sky() {
	viewer->data(sky).show_texture = true;
	viewer->data(sky).set_texture(Rsk, Gsk, Bsk);
}
void SurroundingManager::set_sides() {
	viewer->data(sides).show_texture = true;
	viewer->data(sides).set_texture(Rm, Gm, Bm);
}
void SurroundingManager::defaulground() {
	viewer->data(ground).show_texture = false;
}
void SurroundingManager::defaulsky() {
	viewer->data(sky).show_texture = false;
}
void SurroundingManager::defaulmountain() {
	viewer->data(sides).show_texture = false;
}

