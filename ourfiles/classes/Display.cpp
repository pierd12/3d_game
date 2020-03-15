

#include <chrono>
#include <thread>

#include "../gl.h"
#include "Display.h"

#include "igl/igl_inline.h"
#include <igl/get_seconds.h>
#include <tutorial\tutorial\sandBox\Game.h>
#include <GL/gl.h>


static void glfw_error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

Display::Display(int windowWidth, int windowHeight, const std::string& title)
{
	bool resizable = true, fullscreen = false;
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	// Load OpenGL and its extensions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to load OpenGL and its extensions\n");
		exit(EXIT_FAILURE);
	}
#if defined(DEBUG) || defined(_DEBUG)
	printf("OpenGL Version %d.%d loaded\n", GLVersion.major, GLVersion.minor);
	int major, minor, rev;
	major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
	minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
	rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
	printf("OpenGL version received: %d.%d.%d\n", major, minor, rev);
	printf("Supported OpenGL is %s\n", (const char*)glGetString(GL_VERSION));
	printf("Supported GLSL is %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	//Tamir: changes from here
	// Initialize FormScreen
	 // __viewer = this;
	// Register callbacks
	//glfwSetKeyCallback(window, glfw_key_callback);
	//glfwSetCursorPosCallback(window,glfw_mouse_move);
	//glfwSetScrollCallback(window, glfw_mouse_scroll);
	//glfwSetMouseButtonCallback(window, glfw_mouse_press);
	//glfwSetWindowSizeCallback(window,glfw_window_size);


	//glfwSetCharModsCallback(window,glfw_char_mods_callback);
	//glfwSetDropCallback(window,glfw_drop_callback);
	// Handle retina displays (windows and mac)
	//int width, height;
	//glfwGetFramebufferSize(window, &width, &height);
	//int width_window, height_window;
	//glfwGetWindowSize(window, &width_window, &height_window);
	//highdpi = windowWidth/width_window;

	//glfw_window_size(window,width_window,height_window);
	//opengl.init();
//		core().align_camera_center(data().V, data().F);
		// Initialize IGL viewer
//		init();

}

bool Display::launch_rendering(bool loop)
{
	// glfwMakeContextCurrent(window);
	// Rendering loop
	const int num_extra_frames = 5;
	int frame_counter = 0;
	int windowWidth, windowHeight;
	//main loop
	Renderer* renderer = (Renderer*)glfwGetWindowUserPointer(window);
	igl::opengl::glfw::Viewer* scn = renderer->GetScene();
	scn->init();
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	renderer->post_resize(window, windowWidth, windowHeight);
	
	while (!glfwWindowShouldClose(window))
	{
		if (scn->end_game) {
			renderer->launch_shut(window);
			return true;
		}
		double tic = igl::get_seconds();
		renderer->draw(window);
		glfwSwapBuffers(window);
		if (renderer->core().is_animating || frame_counter++ < num_extra_frames)
		{//motion
			glfwPollEvents();
			// In microseconds
			double duration = 1000000. * (igl::get_seconds() - tic);
			const double min_duration = 1000000. / renderer->core().animation_max_fps;
			//------------------------------------------------------------------------
			if (renderer->GetScene()->animating_ik)
				renderer->GetScene()->animate_ik();
			else if (renderer->GetScene()->animating_collision)
				renderer->GetScene()->animate_collision();
			if (renderer->GetScene()->running_game && (!renderer->GetScene()->pause_game))
				((Game*)(renderer->GetScene()->game))->loop();;
			//------------------------------------------------------------------------
			if (duration < min_duration)
			{
				std::this_thread::sleep_for(std::chrono::microseconds((int)(min_duration - duration)));
			}
		}
		else
		{
			//glfwWaitEvents();
			glfwPollEvents();
			frame_counter = 0;
		}
		if (!loop)
			return !glfwWindowShouldClose(window);

#ifdef __APPLE__
		static bool first_time_hack = true;
		if (first_time_hack) {
			glfwHideWindow(window);
			glfwShowWindow(window);
			first_time_hack = false;
		}
#endif
	}
	return EXIT_SUCCESS;
}

void Display::AddKeyCallBack(void(*keyCallback)(GLFWwindow*, int, int, int, int))
{
	glfwSetKeyCallback(window, (void(*)(GLFWwindow*, int, int, int, int))keyCallback);//{

}
void Display::AddMouseCallBacks(void (*mousebuttonfun)(GLFWwindow*, int, int, int), void (*scrollfun)(GLFWwindow*, double, double), void (*cursorposfun)(GLFWwindow*, double, double))
{
	glfwSetMouseButtonCallback(window, mousebuttonfun);
	glfwSetScrollCallback(window, scrollfun);
	glfwSetCursorPosCallback(window, cursorposfun);
}

void Display::AddResizeCallBack(void (*windowsizefun)(GLFWwindow*, int, int))
{
	glfwSetWindowSizeCallback(window, windowsizefun);
}

void Display::SetRenderer(void* userPointer)
{

	glfwSetWindowUserPointer(window, userPointer);

}

void* Display::GetScene()
{
	return glfwGetWindowUserPointer(window);
}

void Display::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void Display::PollEvents()
{
	glfwPollEvents();
}

Display::~Display()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

