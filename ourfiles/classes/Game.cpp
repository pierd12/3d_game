#include "Game.h"
#include <iostream>
#include <cstdlib>
Game::Game(igl::opengl::glfw::Viewer* viewer, CollisionManager* coll_manager) {
	this->sound = new SoundManager();
	this->viewer = viewer;
	this->snake = new Snake(11, viewer);
	this->enemy_manager = new EnemyManager(2, 2, 2, viewer);
	this->coll_manager = coll_manager;
	this->surr = new SurroundingManager(viewer);

}
void Game::init() {
	this->enemies = 6;
	this->current_enemies = 4;
	this->level = 0;
	this->score = 0;
	this->isOn = false;
	this->max_level = 5;
	coll_manager->init(viewer);
	enemy_manager->init();
	snake->init();
	enemy_manager->set_surr_index(surr->surr_index_special);
	viewer->picked = true;
}
void Game::init_score(std::string name) {
	std::fstream infile;
	infile.open("score.txt");
	if (!infile) {
		std::cout << "Can't open file\n";
		return;
	}
	std::string st_score;
	std::string st_user;
	getline(infile, st_score);
	getline(infile, st_user);
	std::string::size_type sz;
	this->high_score = std::stoi(st_score, &sz);
	this->old_user_name = st_user;
	infile.close();
	this->user_name = name;
	std::cout << user_name << std::endl;

}
void Game::setGround(int type) {
	if (type == 1) {
		surr->set_grass();
	}
	if (type == 0) {
		surr->defaulground();
	}
	if (type == 2) {
		surr->set_sand();
	}
}
void Game::setSky(int type) {
	if (type == 1) {
		surr->set_sky();
	}
	if (type == 0) {
		surr->defaulsky();
	}
	
}
void Game::setMountain(int type) {
	if (type == 1) {
		//surr->mountain();
	}
	if (type == 0) {
		//surr->defaulmountain();
	}
}
void Game::setSound() {
	sound->isOn = true;
	if (level >= 2) {
		sound->special();
		return;
	}
	sound->backgroundSound();
}
void Game::stopSound() {
	sound->isOn = false;
	sound->stopbackgroundSound();
}
void Game::setVolume(int v) {
	sound->setVolume(v);
}

Game::~Game() {
	std::cout << "ending game..." << std::endl;
}

void Game::set_camera() {

	snake->set_camera_on_head();
}

void Game::loop() {
	if (snake->is_snake_selected(viewer->selected_data_index) || viewer->picked || !running) {
		if (!snake->reached(snake->initial_tip_position))
			snake->move_to(snake->initial_tip_position);
	}
	// if enemy selected
	else if (enemy_manager->is_enemy(viewer->selected_data_index)) {
		Eigen::Vector3f position = enemy_manager->get_enemy_position(viewer->selected_data_index);
		if (!coll_manager->detect_collision(viewer, viewer->selected_data_index, enemy_manager->get_type(viewer->selected_data_index))) {
			if (snake->can_reach(position)) {
				sound->attack();
				snake->move_to(position);
			}
			else {
				sound->miss();
				viewer->picked = true;
			}
		}
		else {//snake reached!
			enemy_manager->deactivate_enemy(viewer->selected_data_index);
			current_enemies--;
			score++;
			viewer->picked = true;
		}
	}
	else {//otherwise
		viewer->picked = true;
	}
	//enemy_manager->manage_speacial_event();
	current_enemies -= enemy_manager->manage_enemies();
}

void Game::add_mouse() {
	//enemy_manager->leveloneupdate();
	if (level <= 2 && level >0 && current_enemies > 0)
		enemy_manager->activate_enemy(current_enemies - 1);
}

void Game::set_level() {
	level++;
	sound->levelup();
	current_enemies = enemies;
	enemy_manager->set_level(level);
	if (level == 3) {
		//sound->special();
	}
}

void Game::finish_game(int score) {
	if (gotOnce)
		return;
	gotOnce = true;
	std::fstream infile;
	infile.open("score.txt");
	if (!infile) {
		std::cout << "Can't open file\n";
		return;
	}
	std::string st_score;
	infile << std::to_string(score);
	infile << "\n";
	infile << this->user_name;
	infile << "\n";
	infile.close();
	sound->Ending();
}
