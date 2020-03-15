#pragma once
#include <igl\opengl\glfw\Viewer.h>
class SurroundingManager
{
public:
	SurroundingManager(igl::opengl::glfw::Viewer* viewer);
	~SurroundingManager();
	void set_grass();
	void set_sand();
	void set_sky();
	void set_sides();
	void defaulground();
	void defaulsky();
	void defaulmountain();
	void deleteSurr();
	igl::opengl::glfw::Viewer* viewer;
	Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> Rg, Gg, Bg, Ag;
	Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> Rs, Gs, Bs, As;
	Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> Rsk, Gsk, Bsk, Ask;
	Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> Rm, Gm, Bm, Am;
	int ground;
	int sky;
	int sides;
	int surr_index_special;
};

