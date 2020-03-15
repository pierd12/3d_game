
#include "igl/opengl/glfw/renderer.h"
#include "tutorial/sandBox/inputManager.h"
#include <tutorial\tutorial\sandBox\StartMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <windows.h>
#include <mmsystem.h>
int main(int argc, char* argv[])
{
	//-----------------------------------
	bool enable_simplefication = false;		// enables and disables simplefication option
	bool render_arm_ass3 = false;					// starts IK animation
	bool collision_detection_ass4 = false;// starts collision detection simulation
	bool snake_game = true;								// starts snake game
	//-----------------------------------
	Display* disp = new Display(1400, 800, "Final Project");
	Renderer renderer;
	igl::opengl::glfw::Viewer viewer;
	igl::opengl::glfw::imgui::ImGuiMenu menu;
	StartMenu* start = new StartMenu(&viewer, &menu);
	//viewer.init_game();
	Init(*disp);
	renderer.init(&viewer);
	//viewer.set_second_camera();
	disp->SetRenderer(&renderer);
	viewer.setWindow(disp->window);
	viewer.setRenderer(&renderer);
	disp->launch_rendering(true);
	delete disp;
}
