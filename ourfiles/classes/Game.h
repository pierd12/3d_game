#pragma once
#include <tutorial\tutorial\sandBox\Snake.h>
#include <tutorial\tutorial\sandBox\EnemyManager.h>
#include <tutorial\tutorial\sandBox\CollisionManager.h>
#include <tutorial\tutorial\sandBox\SoundManager.h>
#include <tutorial\tutorial\sandBox\SurroundingManager.h>

class Game
{
public:
	Game(igl::opengl::glfw::Viewer* viewer, CollisionManager* coll_manager);//;, Snake* snake, EnemyManager*mouse_manager, CollisionManager* coll_manager);
	~Game();
	void add_mouse();
	void init();
	void init_score(std::string name);
	void set_camera();
	void set_level();
	void setGround(int type);
	void setSky(int type);
	void setMountain(int type);
	void setSound();
	void stopSound();
	void setVolume(int v);

	void finish_game(int score);
	void loop();
	int high_score;
	int score;
	int level;
	int max_level ;
	int enemies;
	int current_enemies;
	bool running = true;
	bool isOn;
	bool gotOnce = false;
	std::string user_name = "";
	std::string old_user_name = "";
	EnemyManager* enemy_manager;
private:
	
	Snake* snake;
	//EnemyManager* enemy_manager;
	CollisionManager* coll_manager;
	SoundManager* sound;
	igl::opengl::glfw::Viewer* viewer;
	SurroundingManager* surr;
	
};

