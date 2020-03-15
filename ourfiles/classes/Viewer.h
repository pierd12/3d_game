// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2014 Daniele Panozzo <daniele.panozzo@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#ifndef IGL_OPENGL_GLFW_VIEWER_H
#define IGL_OPENGL_GLFW_VIEWER_H

#ifndef IGL_OPENGL_4
#define IGL_OPENGL_4
#endif

#include "../../igl_inline.h"
#include "../MeshGL.h"
#include "../ViewerCore.h"
#include "../ViewerData.h"
#include "ViewerPlugin.h"
#include "./renderer.h"
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <vector>
#include <string>
#include <cstdint>
#include <igl\opengl\ViewerCore.h>
#include <igl\AABB.h>
#define IGL_MOD_SHIFT           0x0001
#define IGL_MOD_CONTROL         0x0002
#define IGL_MOD_ALT             0x0004
#define IGL_MOD_SUPER           0x0008

struct GLFWwindow;

namespace igl
{
	namespace opengl
	{
		namespace glfw
		{
			// GLFW-based mesh viewer
			class Viewer : public Movable
			{
			public:
				
				// added functions ------------------------------------------------------------
					//-----------------------(Assignments 1 & 2)---------------------------------
				IGL_INLINE bool load_mesh_from_configuration(bool enable_simplefication, bool render_arm, bool collision_detection_ass4);
				IGL_INLINE bool simpleficate_mesh(int);
				IGL_INLINE void init_simplefication_objs();
				IGL_INLINE bool collapse(
					const int e,
					const Eigen::RowVectorXd& p,
					Eigen::MatrixXd& V,
					Eigen::MatrixXi& F,
					Eigen::MatrixXi& E,
					Eigen::VectorXi& EMAP,
					Eigen::MatrixXi& EF,
					Eigen::MatrixXi& EI,
					std::vector<Eigen::Matrix<double, 4, 4>>& Qs,
					int& a_e1,
					int& a_e2,
					int& a_f1,
					int& a_f2);
				IGL_INLINE bool my_collapse_edge(
					const std::function<void(
						std::vector<Eigen::Matrix<double, 4, 4>> & Qs,
						const int,
						const Eigen::MatrixXd&,
						const Eigen::MatrixXi&,
						const Eigen::MatrixXi&,
						const Eigen::VectorXi&,
						const Eigen::MatrixXi&,
						const Eigen::MatrixXi&,
						double&,
						Eigen::RowVectorXd&)>& cost_and_placement,
					std::vector<Eigen::Matrix<double, 4, 4>>& Qs,
					Eigen::MatrixXd& V,
					Eigen::MatrixXi& F,
					Eigen::MatrixXi& E,
					Eigen::VectorXi& EMAP,
					Eigen::MatrixXi& EF,
					Eigen::MatrixXi& EI,
					std::set<std::pair<double, int> >& Q,
					std::vector<std::set<std::pair<double, int> >::iterator >& Qit,
					Eigen::MatrixXd& C);
				//----------------------------(Assignment 3)-----------------------------------
				IGL_INLINE void init_link_and_sphere(void);
				IGL_INLINE void animate_ik();
				IGL_INLINE Eigen::Matrix4f get_parent_link_T(int index);
				IGL_INLINE Eigen::Matrix4f get_parent_link_Rot(int index);
				//----------------------------(Assignment 4)-----------------------------------
				IGL_INLINE void init_collision();
				IGL_INLINE void reset_collision();
				IGL_INLINE void animate_collision();
				IGL_INLINE bool detect_collision(igl::AABB<Eigen::MatrixXd, 3>*, igl::AABB<Eigen::MatrixXd, 3>*, Eigen::Vector3d A[], Eigen::Vector3d B[]);
				IGL_INLINE bool can_seperate(Eigen::AlignedBox<double, 3>&, Eigen::AlignedBox<double, 3>&, Eigen::Vector3d A[], Eigen::Vector3d B[]);
				IGL_INLINE void end_collision_simulation(void);
				IGL_INLINE void draw_m_box(int index, Eigen::AlignedBox<double, 3>&, Eigen::RowVector3d color);
				//----------------------------(Final Project)-----------------------------------
				IGL_INLINE void init_game();
				IGL_INLINE void set_second_camera();
				
				//-------------imgui-------------------------------------------------------------


				IGL_INLINE void setWindow(GLFWwindow* window);
				IGL_INLINE void setRenderer(void* render);
				IGL_INLINE igl::opengl::ViewerCore& core(unsigned core_id = 0);
				IGL_INLINE const igl::opengl::ViewerCore& core(unsigned core_id = 0) const;
				IGL_INLINE void snap_to_canonical_quaternion();
				IGL_INLINE void init_plugins();
				IGL_INLINE void shutdown_plugins();
				//---------------------------------------------------------------------------------


				Viewer();
				~Viewer();
				// Mesh IO
				
				IGL_INLINE void init();
				//IGL_INLINE void init_plugins();
				//IGL_INLINE void shutdown_plugins();
				// Scene IO
				IGL_INLINE bool load_mesh_from_file(const std::string& mesh_file_name);
				IGL_INLINE bool   save_mesh_to_file(const std::string& mesh_file_name);
				IGL_INLINE bool load_scene();
				IGL_INLINE bool load_scene(std::string fname);
				IGL_INLINE bool save_scene();
				IGL_INLINE bool save_scene(std::string fname);
				//----------------------------------------------------------------------------------------------------
				IGL_INLINE void open_dialog_load_mesh();
				IGL_INLINE void open_dialog_save_mesh();
				IGL_INLINE ViewerData& data(int mesh_id = -1);
				IGL_INLINE const ViewerData& data(int mesh_id = -1) const;
				IGL_INLINE int append_mesh(bool visible = true);
				IGL_INLINE bool erase_mesh(const size_t index);
				IGL_INLINE size_t mesh_index(const int id) const;
			public:
				std::vector<ViewerData> data_list;

				size_t selected_data_index;
				int next_data_id;
				bool enable_semplefication = false;
				bool picked = false;
				bool assignment3 = false;
				bool assignment4 = false;
				bool animating_ik = false;
				bool animating_collision = false;
				int link_number = 4;
				int root_link_index;
				int last_link_index;
				int sphere_index;
				std::vector< igl::AABB<Eigen::MatrixXd, 3>*> tree_roots;
				std::vector< igl::AABB<Eigen::MatrixXd, 3>*> trees;
				std::vector<Eigen::AlignedBox<double, 3>*> last_box;
				//****************************************************************
				//**********************FINAL PROJECT DATA************************
				bool running_game = false;
				bool pause_game = false;
				bool finish_game = false;
				bool end_game = false;
				void* game;
				std::string* data_path;
				std::string cyl = "/ycylinder.obj";
				std::string sph = "/sphere.obj";
				int frames = 0;
				int left_view;
				int right_view;
				igl::opengl::ViewerCore* second_camera;
				//--------------/for imgui------------------------
				GLFWwindow* window;
				std::vector<ViewerPlugin*> plugins;
				//void* renderer;
				float scroll_position;
			public:
				EIGEN_MAKE_ALIGNED_OPERATOR_NEW
			};

		} // end namespace
	} // end namespace
} // end namespace

#ifndef IGL_STATIC_LIBRARY
#  include "Viewer.cpp"
#endif

#endif