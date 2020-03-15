#pragma once
#include <vector>
#include <igl\opengl\ViewerData.h>
#include <igl\opengl\glfw\Viewer.h>


#define SPHERE 0
#define CUBE 1
#define BUNNY 2
#define SSPHERE 3

class EnemyManager
{
public:
	EnemyManager(int cube_amount, int sphere_amount, int bunny_amount, igl::opengl::glfw::Viewer* viewer);
	~EnemyManager();
	Eigen::Vector3f randomNoGenerator();
	Eigen::Vector3f srandomNoGenerator();
	void init();
	void intro_init();
	void speacial_init();
	void special_event_init();
	
	void set_surr_index(int surr);
	void set_level(int level);
	void set_speacial_level(int level);
	void set_special_event();
	int manage_enemies();
	void manage_speacial_event();
	void manage_y(int enemy);
	void manage_x(int enemy);
	void manage_z(int enemy);
	
	Eigen::Vector3f get_enemy_position(int index);
	bool is_enemy(int index);
	void activate_enemy(int index);
	void deactivate_enemy(int index);
	int get_type(int index);
	void reset_position(int index);
	void parse_data(std::string enemy_data, Eigen::Vector3f* initial_position, Eigen::Vector3f* initial_speed);
	float accs;
	float max_speed;
private:
	int level = 0;
	int amount;
	int first_one;
	int last_one;
	int first_enemy_index;
	int last_enemy_index;
	int first_enemy_index_special;
	int last_enemy_index_special;
	int surr_index_special;
	float gravity_force = -0.005;
	int cube_amount;
	int sphere_amount;
	int bunny_amount;
	Eigen::Vector3f gravity;
	igl::opengl::glfw::Viewer* viewer;
	Eigen::Vector3f deactivated_position;
	//vectors
	std::vector<bool> active_enemy;
	std::vector<bool> fall;
	std::vector<bool> turn;
	std::vector<bool> forward;
	std::vector<Eigen::Vector3f> enemy_speed;
	std::vector<Eigen::Vector3f> enemy_position;
	std::vector<Eigen::Vector3f> enemy_position_special;
	std::vector<int> enemy_type;
};

