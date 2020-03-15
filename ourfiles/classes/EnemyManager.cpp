#include "EnemyManager.h"
#include <cstdlib>
#include <iostream>
#include <igl/png/readPNG.h>
EnemyManager::EnemyManager(int cube_amount, int sphere_amount, int bunny_amount, igl::opengl::glfw::Viewer* viewer) {
	this->amount = cube_amount + sphere_amount + bunny_amount;
	this->bunny_amount = bunny_amount;
	this->cube_amount = cube_amount;
	this->sphere_amount = sphere_amount;
	this->viewer = viewer;
}
EnemyManager::~EnemyManager() {}

Eigen::Vector3f EnemyManager::randomNoGenerator()
{
	int rand();
	int x =  (rand() % 20) - 10;
	int z = (rand() % 20) - 10;
	return Eigen::Vector3f(x, 0, z);
}
Eigen::Vector3f EnemyManager::srandomNoGenerator()
{
	int rand();
	int x = (rand() % 38) - 18;
	int y = (rand() % 18);
	int z = (rand() % 40) - 20;
	return Eigen::Vector3f(x, y, z);
}

void EnemyManager::init() {
	gravity = Eigen::Vector3f(0, gravity_force, 0);
	intro_init();
	speacial_init();
	special_event_init();
	this->accs = 1.1;
	this->max_speed = 1;
}
void EnemyManager::intro_init() {
	for (int enemy = 0; enemy < 4; enemy++) {
		enemy_type.push_back(CUBE);
		viewer->load_mesh_from_file(*viewer->data_path + "/cube.obj");
		if (enemy == 0) first_enemy_index = viewer->selected_data_index;
		if (enemy == 3) last_enemy_index = viewer->selected_data_index;
		Eigen::Vector3f initial_position = randomNoGenerator();
		Eigen::Vector3f initial_speed(0, 0, 0);
		active_enemy.push_back(true);
		enemy_speed.push_back(initial_speed);
		enemy_position.push_back(initial_position);
		viewer->data().MyTranslate(initial_position, false);
	}
}
void EnemyManager::speacial_init() {
	deactivated_position = Eigen::Vector3f(0, -5, 0);
	for (int enemy = 0; enemy < amount; enemy++) {
		if (enemy < cube_amount) {
			enemy_type.push_back(CUBE);
			viewer->load_mesh_from_file(*viewer->data_path + "/cube.obj");
		}
		else if (enemy >= cube_amount && enemy < cube_amount + sphere_amount) {
			enemy_type.push_back(SPHERE);
			viewer->load_mesh_from_file(*viewer->data_path + "/sphere.obj");
		}
		else {
			enemy_type.push_back(BUNNY);
			viewer->load_mesh_from_file(*viewer->data_path + "/bunny.off");
			viewer->data().MyScale(Eigen::Vector3f(10, 10, 10));
		}
		if (enemy == 0) first_one = viewer->selected_data_index;
		if (enemy == amount - 1) last_one = viewer->selected_data_index;
		viewer->data().MyTranslate(deactivated_position, true);
	}
}
void EnemyManager::special_event_init() {
	for (int enemy = 0; enemy < 4; enemy++) {
		enemy_type.push_back(SSPHERE);
		viewer->load_mesh_from_file(*viewer->data_path + "/sphere.obj");
		viewer->data().MyScale(Eigen::Vector3f(2, 2, 2));
		if (enemy == 0) first_enemy_index_special = viewer->selected_data_index;
		if (enemy == 3) last_enemy_index_special = viewer->selected_data_index;
		viewer->data().MyTranslate(Eigen::Vector3f(400, 0, 0), true);
		Eigen::Vector3f initial_position = srandomNoGenerator();
		enemy_position_special.push_back(initial_position);
	}
}
void EnemyManager::set_surr_index(int surr) {
	this->surr_index_special = surr;
}
void EnemyManager::set_level(int level) {
	this->level = level;
	std::cout << "new Level!!\n";
	if (this->level <= 2) {
		set_speacial_level(level);
	}
	else {
		set_special_event();
	}

}
void EnemyManager::set_special_event() {
	first_enemy_index = first_enemy_index_special;
	last_enemy_index = last_enemy_index_special;
	enemy_position.clear();
	enemy_speed.clear();
	active_enemy.clear();
	//right
	viewer->data(surr_index_special).MyTranslate(Eigen::Vector3f(-360, 0, 0), true);
	///front
	viewer->data(surr_index_special+1).MyTranslate(Eigen::Vector3f(-400, 0, -30), true);
	///left
	viewer->data(surr_index_special+2).MyTranslate(Eigen::Vector3f(-440, 0, 0), true);
	//up
	viewer->data(surr_index_special+3).MyTranslate(Eigen::Vector3f(-400, 40, 0), true);
	for (int enemy = 0; enemy < 4; enemy++) {
		Eigen::Vector3f initial_position = enemy_position_special.at(enemy)+Eigen::Vector3f(-400,0,0);
		Eigen::Vector3f initial_speed(-0.05, -0.05, -0.05);
		active_enemy.push_back(true);
		enemy_speed.push_back(initial_speed);
		enemy_position.push_back(initial_position);
		fall.push_back(true);
		turn.push_back(true);
		forward.push_back(true);
		viewer->data(first_enemy_index+enemy).MyTranslate(initial_position, true);
	}
}
void EnemyManager::set_speacial_level(int level) {
	first_enemy_index = first_one;
	last_enemy_index = last_one;
	enemy_position.clear();
	enemy_speed.clear();
	active_enemy.clear();
	//enemy_type.clear();
	std::fstream infile;
	std::string enemy_data;
	std::string levelstr = "level" + std::to_string(level) + ".txt";
	infile.open(levelstr);
	if (!infile) {
		std::cout << "Can't open file" + levelstr;
	}
	else {
		for (int enemy = 0; enemy < amount; enemy++) {
			//deactivate_enemy(enemy+first_enemy_index);
			active_enemy.push_back(false);
			getline(infile, enemy_data);
			Eigen::Vector3f initial_position;
			Eigen::Vector3f initial_speed;

			parse_data(enemy_data, &initial_position, &initial_speed);
			active_enemy.push_back(false);
			enemy_speed.push_back(initial_speed);
			enemy_position.push_back(initial_position);
			viewer->data().MyTranslate(deactivated_position, true);
		}
	}
	infile.close();
}

int EnemyManager::manage_enemies() {
	int missed = 0;
	if (level <= 2 && level > 0) {
		for (int enemy = first_enemy_index; enemy < last_enemy_index + 1; enemy++) {
			if (active_enemy.at(enemy - first_enemy_index)) {
				if (get_enemy_position(enemy)(1) < -1) {
					deactivate_enemy(enemy);
					missed++;
				}
				else viewer->data_list[enemy].MyTranslate((enemy_speed.at(enemy - first_enemy_index) += gravity), false);
			}
		}
	}
	if (level >= 3) manage_speacial_event();
	return missed;
}
void EnemyManager::manage_speacial_event() {
	for (int enemy = first_enemy_index; enemy < last_enemy_index + 1; enemy++) {
		manage_x(enemy);
		manage_y(enemy);
		manage_z(enemy);
	}
}
void EnemyManager::manage_x(int enemy) {
	if (get_enemy_position(enemy)(0) > -18 && (turn.at(enemy - first_enemy_index))) {
		viewer->data(enemy).MyTranslate(enemy_speed.at(enemy - first_enemy_index), true);
	}
	else {
		Eigen::Vector3f f = enemy_speed.at(enemy - first_enemy_index);
		if (get_enemy_position(enemy)(0) <= -18 && (turn.at(enemy - first_enemy_index))) {
			float x = std::min(max_speed, -( (enemy_speed.at(enemy - first_enemy_index)(0)) *accs));
			enemy_speed.at(enemy - first_enemy_index) =
				Eigen::Vector3f(x,
					enemy_speed.at(enemy - first_enemy_index)(1), enemy_speed.at(enemy - first_enemy_index)(2));
			turn.at(enemy - first_enemy_index) = false;
		}
		else {
			if (get_enemy_position(enemy)(0) < 18 && !(turn.at(enemy - first_enemy_index))) {
				viewer->data(enemy).MyTranslate(f, true);
			}
			else {
				turn.at(enemy - first_enemy_index) = true;
				float x = std::max(-max_speed, -((enemy_speed.at(enemy - first_enemy_index)(0)) * accs));
				enemy_speed.at(enemy - first_enemy_index) =
					Eigen::Vector3f(x,
						enemy_speed.at(enemy - first_enemy_index)(1), enemy_speed.at(enemy - first_enemy_index)(2));
			}
		}
	}
}
void EnemyManager::manage_y(int enemy) {
	if (get_enemy_position(enemy)(1) > 1 && (fall.at(enemy - first_enemy_index))) {
		viewer->data(enemy).MyTranslate(enemy_speed.at(enemy - first_enemy_index), true);
	}
	else {
		Eigen::Vector3f f = enemy_speed.at(enemy - first_enemy_index);
		if (get_enemy_position(enemy)(1) <= 1 && (fall.at(enemy - first_enemy_index))) {
			float y = std::min(max_speed, -((enemy_speed.at(enemy - first_enemy_index)(1)) * accs));
			enemy_speed.at(enemy - first_enemy_index) =
				Eigen::Vector3f(enemy_speed.at(enemy - first_enemy_index)(0),
					y, enemy_speed.at(enemy - first_enemy_index)(2));
			fall.at(enemy - first_enemy_index) = false;
		}
		else {
			if (get_enemy_position(enemy)(1) < 18 && !(fall.at(enemy - first_enemy_index))) {
				viewer->data(enemy).MyTranslate(f, true);
			}
			else {
				fall.at(enemy - first_enemy_index) = true;
				float y = std::max(-max_speed, -((enemy_speed.at(enemy - first_enemy_index)(1)) * accs));
				enemy_speed.at(enemy - first_enemy_index) =
					Eigen::Vector3f(enemy_speed.at(enemy - first_enemy_index)(0),
						y, enemy_speed.at(enemy - first_enemy_index)(2));
			}
		}
	}
}
void EnemyManager::manage_z(int enemy) {
	if (get_enemy_position(enemy)(2) > -20 && (forward.at(enemy - first_enemy_index))) {
		viewer->data(enemy).MyTranslate(enemy_speed.at(enemy - first_enemy_index), true);
	}
	else {
		Eigen::Vector3f f = enemy_speed.at(enemy - first_enemy_index);
		if (get_enemy_position(enemy)(2) <= -20 && (forward.at(enemy - first_enemy_index))) {
			float z = std::min(max_speed, -((enemy_speed.at(enemy - first_enemy_index)(2)) * accs));
			enemy_speed.at(enemy - first_enemy_index) =
				Eigen::Vector3f(enemy_speed.at(enemy - first_enemy_index)(0),
					enemy_speed.at(enemy - first_enemy_index)(1), z);
			forward.at(enemy - first_enemy_index) = false;
		}
		else {
			if (get_enemy_position(enemy)(2) < 20 && !(forward.at(enemy - first_enemy_index))) {
				viewer->data(enemy).MyTranslate(f, true);
			}
			else {
				forward.at(enemy - first_enemy_index) = true;
				float z = std::max(-max_speed, -((enemy_speed.at(enemy - first_enemy_index)(2)) * accs));
				enemy_speed.at(enemy - first_enemy_index) =
					Eigen::Vector3f(enemy_speed.at(enemy - first_enemy_index)(0),
						enemy_speed.at(enemy - first_enemy_index)(1), z);
			}
		}
	}
}

Eigen::Vector3f EnemyManager::get_enemy_position(int index) {
	Eigen::Vector3f position = (viewer->data(index).MakeTrans() * Eigen::Vector4f(0, 0, 0, 1)).block<3, 1>(0, 0);
	return position;
}

bool EnemyManager::is_enemy(int index) {
	if (index >= first_enemy_index && index <= last_enemy_index) return active_enemy.at(index - first_enemy_index);
	return false;
}

void EnemyManager::activate_enemy(int index) {
	std::cout << "activating enemy : " << index << std::endl;
	if (!active_enemy.at(index)) {
		active_enemy.at(index) = true;
		viewer->data_list[index + first_enemy_index].MyTranslate(enemy_position.at(index), true);
	}
}

void EnemyManager::deactivate_enemy(int index) {
	if (level <= 2 && level > 0) {
		if (!(index >= first_enemy_index && index <= last_enemy_index)) return;
		active_enemy.at(index - first_enemy_index) = false;
		reset_position(index);
		//viewer->data_list[index + first_enemy_index].MyTranslate(-get_enemy_position(index,viewer), true);
	}
	else {
		if (level == 0) {
			if (!(index >= first_enemy_index && index <= last_enemy_index)) return;
			viewer->data(index).clear();
		}
	}
}

void EnemyManager::reset_position(int index) {
	viewer->data_list[index].MyTranslate(-get_enemy_position(index), true);
	viewer->data_list[index].MyTranslate(deactivated_position, true);
	enemy_speed.at(index - first_enemy_index) = Eigen::Vector3f(0, 0, 0.1);
}

void EnemyManager::parse_data(std::string enemy_data, Eigen::Vector3f* initial_position, Eigen::Vector3f* initial_speed) {
	int i = -1;
	int j = 0;
	for (int index = 0; index < amount; index++) {
		j = enemy_data.find_first_of(" ", i + 1);
		if (index < 3) (*initial_position)(index) = std::stof(enemy_data.substr(i + 1, j - i - 1));
		else (*initial_speed)(index - 3) = std::stof(enemy_data.substr(i + 1, j - i - 1));
		i = j;
	}
}

int EnemyManager::get_type(int index) {
	return enemy_type.at(index - first_enemy_index);
}