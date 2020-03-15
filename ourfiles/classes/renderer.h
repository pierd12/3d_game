#pragma once
#include <igl/igl_inline.h>
#include <vector>
#include <functional>
#include <igl/opengl/ViewerCore.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/ViewerPlugin.h>
struct GLFWwindow;

class Renderer
{
public:
	Renderer();
	~Renderer();
	IGL_INLINE void draw(GLFWwindow* window);
	IGL_INLINE void launch_shut(GLFWwindow* window);
	IGL_INLINE void StartGame(igl::opengl::glfw::Viewer* viewer);
	IGL_INLINE void init(igl::opengl::glfw::Viewer* scn);

	//IGL_INLINE bool key_pressed(unsigned int unicode_key, int modifiers);

		// Returns **true** if action should be cancelled.
	std::function<bool(GLFWwindow * window)> callback_init;
	std::function<bool(GLFWwindow * window)> callback_pre_draw;
	std::function<bool(GLFWwindow * window)> callback_post_draw;
	std::function<bool(GLFWwindow * window, int button, int modifier)> callback_mouse_down;
	std::function<bool(GLFWwindow * window, int button, int modifier)> callback_mouse_up;
	std::function<bool(GLFWwindow * window, int mouse_x, int mouse_y)> callback_mouse_move;
	std::function<bool(GLFWwindow * window, float delta_y)> callback_mouse_scroll;
	std::function<bool(GLFWwindow * window, unsigned int key, int modifiers)> callback_key_pressed;
	std::function<bool(GLFWwindow * window, int w, int h)> callback_post_resize;
	// THESE SHOULD BE DEPRECATED:
	std::function<bool(GLFWwindow * window, unsigned int key, int modifiers)> callback_key_down;
	std::function<bool(GLFWwindow * window, unsigned int key, int modifiers)> callback_key_up;
	// Pointers to per-callback data
	void* callback_init_data;
	void* callback_pre_draw_data;
	void* callback_post_draw_data;
	void* callback_mouse_down_data;
	void* callback_mouse_up_data;
	void* callback_mouse_move_data;
	void* callback_mouse_scroll_data;
	void* callback_key_pressed_data;
	void* callback_key_down_data;
	void* callback_key_up_data;

	IGL_INLINE igl::opengl::ViewerCore& core(unsigned core_id = 0);
	IGL_INLINE const igl::opengl::ViewerCore& core(unsigned core_id = 0) const;
	IGL_INLINE int append_core(Eigen::Vector4f viewport, bool append_empty = false);
	IGL_INLINE bool erase_core(const size_t index);
	IGL_INLINE size_t core_index(const int id) const;

	bool Picking(double x, double y);

	IGL_INLINE bool key_pressed(unsigned int unicode_key, int modifier);
	IGL_INLINE void resize(GLFWwindow* window, int w, int h); // explicitly set window size
	IGL_INLINE void post_resize(GLFWwindow* window, int w, int h); // external resize due to user interaction
	void SetScene(igl::opengl::glfw::Viewer* scn);
	void UpdatePosition(double xpos, double ypos);
	void MouseProcessing(int button);
	inline igl::opengl::glfw::Viewer* GetScene() {
		return scn;
	}
	inline void ChangeCamera(int unicode_key)
	{
		selected_core_index =
			(selected_core_index + core_list.size() + (unicode_key == ']' ? 1 : -1)) % core_list.size();

	}
	float get_currently_picked_object_z(double x, double y);

	IGL_INLINE void snap_to_canonical_quaternion();
	// List of registered plugins
	igl::opengl::glfw::Viewer* scn;
	
private:
	// Stores all the viewing options
	std::vector<igl::opengl::ViewerCore> core_list;
	size_t selected_core_index;
	int next_core_id;
	float highdpi;
	double xold, yold, xrel, yrel;
	int left_view;
	int right_view;
	int w;
};

