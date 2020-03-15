#include "igl/opengl/glfw/renderer.h"

#include <GLFW/glfw3.h>
#include <igl/unproject_onto_mesh.h>
#include "igl/look_at.h"
#include <Eigen/Dense>
#include <igl/snap_to_canonical_view_quat.h>

//-----------------------------------


Renderer::Renderer() : selected_core_index(0),
next_core_id(2)
{
	core_list.emplace_back(igl::opengl::ViewerCore());
	core_list.front().id = 1;
	// C-style callbacks
	callback_init = nullptr;
	callback_pre_draw = nullptr;
	callback_post_draw = nullptr;
	callback_mouse_down = nullptr;
	callback_mouse_up = nullptr;
	callback_mouse_move = nullptr;
	callback_mouse_scroll = nullptr;
	callback_key_down = nullptr;
	callback_key_up = nullptr;

	callback_init_data = nullptr;
	callback_pre_draw_data = nullptr;
	callback_post_draw_data = nullptr;
	callback_mouse_down_data = nullptr;
	callback_mouse_up_data = nullptr;
	callback_mouse_move_data = nullptr;
	callback_mouse_scroll_data = nullptr;
	callback_key_down_data = nullptr;
	callback_key_up_data = nullptr;
	highdpi = 1;

	xold = 0;
	yold = 0;

}

IGL_INLINE void Renderer::draw(GLFWwindow* window)
{
	using namespace std;
	using namespace Eigen;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	int width_window, height_window;
	glfwGetWindowSize(window, &width_window, &height_window);

	auto highdpi_tmp = (width_window == 0 || width == 0) ? highdpi : (width / width_window);

	if (fabs(highdpi_tmp - highdpi) > 1e-8)
	{
		post_resize(window, width, height);
		highdpi = highdpi_tmp;
	}

	for (auto& core : core_list)
	{
		core.clear_framebuffers();
	}
	for (unsigned int i = 0; i < scn->plugins.size(); ++i)
	{
		if (scn->plugins[i]->pre_draw())
		{
			return;
		}
	}
	if (callback_pre_draw)
	{
		if (callback_pre_draw(window))
		{
			return;
		}
	}
	//std::cout << "----------------------------\n";
	for (auto& core : core_list)
	{
		int mesh_id = 0;
		for (auto& mesh : scn->data_list)
		{
			if (mesh.is_visible & core.id)
			{
				Eigen::Matrix4f m = scn->MakeTrans() * scn->get_parent_link_T(mesh_id);
				
				core.draw(m, mesh);
				mesh_id++;
			}
		}
	}
	
	for (unsigned int i = 0; i <scn->plugins.size(); ++i)
	{
		if (scn->plugins[i]->post_draw())
		{
			break;
		}
	}
	if (callback_post_draw)
	{
		if (callback_post_draw(window))
		{
			return;
		}
	}
	//std::cout << "----------------------------\n";
}

IGL_INLINE void Renderer::launch_shut(GLFWwindow* window)
{
	for (auto& data : scn->data_list)
	{
		data.meshgl.free();
	}
	//if (scn)
		//delete scn;
	core().shut(); // Doesn't do anything
	scn->shutdown_plugins();
	//glfwDestroyWindow(window);
	//glfwTerminate();
	return;
}
IGL_INLINE void Renderer::StartGame(igl::opengl::glfw::Viewer* viewer) {
	if (scn->running_game) {
		core().viewport = Eigen::Vector4f(0, 0, 640, 800);
		scn->left_view = core_list[0].id;
		scn->right_view = append_core(Eigen::Vector4f(640, 0, 640, 800));
		viewer->second_camera = &core(2);
		scn->MyTranslate(Eigen::Vector3f(0, -10, -30), true);
		
	}
	core().align_camera_center(scn->data().V, scn->data().F);
}
void Renderer::SetScene(igl::opengl::glfw::Viewer* viewer)
{
	scn = viewer;
}

IGL_INLINE void Renderer::init(igl::opengl::glfw::Viewer* viewer)
{
	scn = viewer;
	core().init();
	core().camera_zoom -= 0.8;
	if (scn->running_game) {
		core().viewport = Eigen::Vector4f(0, 0, 640, 800);
		scn->left_view = core_list[0].id;
		scn->right_view = append_core(Eigen::Vector4f(640, 0, 640, 800));
		viewer->second_camera = &core(2);
		scn->MyTranslate(Eigen::Vector3f(0, -10, -30), true);
		//core().align_camera_center(scn->data().V, scn->data().F);
	}
	//else core().align_camera_center(scn->data().V, scn->data().F);
	core().align_camera_center(scn->data().V, scn->data().F);
}

void Renderer::UpdatePosition(double xpos, double ypos)
{
	xrel = xold - xpos;
	yrel = yold - ypos;
	xold = xpos;
	yold = ypos;
}

void Renderer::MouseProcessing(int button)
{

	if (button == 1)
	{
		if (scn->picked) {
			scn->MyTranslate(Eigen::Vector3f(-xrel / 2000.0f, 0, 0), true);
			scn->MyTranslate(Eigen::Vector3f(0, yrel / 2000.0f, 0), true);
		}
		else {
			if (scn->assignment3 && scn->selected_data_index <= scn->last_link_index && scn->selected_data_index >= scn->root_link_index) {
				scn->data_list[scn->root_link_index].TranslateInSystem(scn->MakeTrans(), Eigen::Vector3f(-xrel / 2000.0f, 0, 0), true);
				scn->data_list[scn->root_link_index].TranslateInSystem(scn->MakeTrans(), Eigen::Vector3f(0, yrel / 2000.0f, 0), true);
			}
			else if (scn->running_game) {
				return;
			}
			else {
				scn->data().TranslateInSystem(scn->MakeTrans(), Eigen::Vector3f(-xrel / 1000.0f, 0, 0), true);
				scn->data().TranslateInSystem(scn->MakeTrans(), Eigen::Vector3f(0, yrel / 1000.0f, 0), true);
			}
		}
	}
	else
	{
		if (scn->picked) {
			scn->MyRotate(Eigen::Vector3f(0, 1, 0), -xrel * 0.3 / 180.0f);
			scn->MyRotate(Eigen::Vector3f(1, 0, 0), -yrel * 0.3 / 180.0f);
		}
		else if (scn->running_game) {
			return;
		}
		else {
			scn->data().MyRotate(Eigen::Vector3f(0, 1, 0), -xrel * 0.3 / 180.0f);
			scn->data().MyRotate(Eigen::Vector3f(1, 0, 0), -yrel * 0.3 / 180.0f);
		}
	}

}

Renderer::~Renderer()
{
	
}

bool Renderer::Picking(double newx, double newy)
{
	int fid;
	//---------------------------------
	//double camera
	if (newx < 800) selected_core_index = 0;
	else selected_core_index = 1;
	//---------------------------------
	//Eigen::MatrixXd C = Eigen::MatrixXd::Constant(scn->data().F.rows(), 3, 1);
	Eigen::Vector3f bc;
	double x = newx;
	double y = core().viewport(3) - newy;
	Eigen::Matrix4f view = Eigen::Matrix4f::Identity();
	igl::look_at(core().camera_eye, core().camera_center, core().camera_up, view);
	view = view * (core().trackball_angle * Eigen::Scaling(core().camera_zoom * core().camera_base_zoom)
		* Eigen::Translation3f(core().camera_translation + core().camera_base_translation)).matrix() * scn->MakeTrans();
	view = view * scn->get_parent_link_T(scn->selected_data_index) * scn->data().MakeTrans();
	if (igl::unproject_onto_mesh(Eigen::Vector2f(x, y), view,
		core().proj, core().viewport, scn->data().V, scn->data().F, fid, bc))
	{
		return true;
	}
	return false;
}

float Renderer::get_currently_picked_object_z(double x, double y1) {
	int fid;
	//---------------------------------
		//double camera
	if (x < 800) selected_core_index = 0;
	else selected_core_index = 1;
	//---------------------------------
	Eigen::Vector3f bc;
	double y = core().viewport(3) - y1;
	Eigen::Matrix4f view = Eigen::Matrix4f::Identity();
	igl::look_at(core().camera_eye, core().camera_center, core().camera_up, view);
	view = view * (core().trackball_angle * Eigen::Scaling(core().camera_zoom * core().camera_base_zoom)
		* Eigen::Translation3f(core().camera_translation + core().camera_base_translation)).matrix() * scn->MakeTrans();
	view = view * scn->get_parent_link_T(scn->selected_data_index) * scn->data().MakeTrans();
	if (igl::unproject_onto_mesh(Eigen::Vector2f(x, y), view,
		core().proj, core().viewport, scn->data().V, scn->data().F, fid, bc))
	{
		auto face = scn->data().F.row(fid);
		auto v0 = scn->data().V.row(face[0]);
		Eigen::Vector4f vv0 = view * Eigen::Vector4f((float)v0[0], (float)v0[1], (float)v0[2], 0.0);
		auto v1 = scn->data().V.row(face[1]);
		Eigen::Vector4f vv1 = view * Eigen::Vector4f((float)v1[0], (float)v1[1], (float)v1[2], 0.0);
		auto v2 = scn->data().V.row(face[2]);
		Eigen::Vector4f vv2 = view * Eigen::Vector4f((float)v2[0], (float)v2[1], (float)v2[2], 0.0);
		float res = ((bc[0] * vv0) + (bc[1] * vv1) + (bc[2] * vv2))[2];
		return  res;
	}
	return 0;
}

IGL_INLINE void Renderer::resize(GLFWwindow* window, int w, int h)
{
	if (window) {
		glfwSetWindowSize(window, w / highdpi, h / highdpi);
	}
	post_resize(window, w, h);
}

IGL_INLINE void Renderer::post_resize(GLFWwindow* window, int w, int h)
{
	if (core_list.size() == 1)
	{
		core().viewport = Eigen::Vector4f(0, 0, w, h);
		this->w = w;
	}
	else
	{
		// It is up to the user to define the behavior of the post_resize() function
		// when there are multiple viewports (through the `callback_post_resize` callback)
		//this->w = w / 2;
		core(1).viewport = Eigen::Vector4f(0, 0, w/2, h);
		core(2).viewport = Eigen::Vector4f(w/2, 0, w - (w /2), h);
	}
	for (unsigned int i = 0; i < scn->plugins.size(); ++i)
	{
		scn->plugins[i]->post_resize(w, h);
	}
	if (callback_post_resize)
	{
		callback_post_resize(window, w, h);
	}
}

IGL_INLINE igl::opengl::ViewerCore& Renderer::core(unsigned core_id /*= 0*/)
{
	assert(!core_list.empty() && "core_list should never be empty");
	int core_index;
	if (core_id == 0)
		core_index = selected_core_index;
	else
		core_index = this->core_index(core_id);
	assert((core_index >= 0 && core_index < core_list.size()) && "selected_core_index should be in bounds");
	return core_list[core_index];
}

IGL_INLINE const igl::opengl::ViewerCore& Renderer::core(unsigned core_id /*= 0*/) const
{
	assert(!core_list.empty() && "core_list should never be empty");
	int core_index;
	if (core_id == 0)
		core_index = selected_core_index;
	else
		core_index = this->core_index(core_id);
	assert((core_index >= 0 && core_index < core_list.size()) && "selected_core_index should be in bounds");
	return core_list[core_index];
}

IGL_INLINE bool Renderer::erase_core(const size_t index)
{
	assert((index >= 0 && index < core_list.size()) && "index should be in bounds");
	//assert(data_list.size() >= 1);
	if (core_list.size() == 1)
	{
		// Cannot remove last viewport
		return false;
	}
	core_list[index].shut(); // does nothing
	core_list.erase(core_list.begin() + index);
	if (selected_core_index >= index && selected_core_index > 0)
	{
		selected_core_index--;
	}
	return true;
}

IGL_INLINE size_t Renderer::core_index(const int id) const {
	for (size_t i = 0; i < core_list.size(); ++i)
	{
		if (core_list[i].id == id)
			return i;
	}
	return 0;
}

IGL_INLINE int Renderer::append_core(Eigen::Vector4f viewport, bool append_empty /*= false*/)
{
	core_list.push_back(core()); // copies the previous active core and only changes the viewport
	core_list.back().viewport = viewport;
	core_list.back().id = next_core_id;
	next_core_id <<= 1;
	if (!append_empty)
	{
		for (auto& data : scn->data_list)
		{
			data.set_visible(true, core_list.back().id);
			data.copy_options(&core(), &core_list.back());
		}
	}
	selected_core_index = core_list.size() - 1;
	return core_list.back().id;
}

IGL_INLINE void Renderer::snap_to_canonical_quaternion()
{
	Eigen::Quaternionf snapq = this->core().trackball_angle;
	igl::snap_to_canonical_view_quat(snapq, 1.0f, this->core().trackball_angle);
}
