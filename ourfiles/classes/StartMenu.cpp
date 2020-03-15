#include "StartMenu.h"
#include <tutorial\tutorial\sandBox\Game.h>

#include <iostream>
#include <stb_image.h>
static int rating = 0;
static int grade = 0;
static bool submitted = false;
StartMenu::StartMenu(igl::opengl::glfw::Viewer* viewer,
	igl::opengl::glfw::imgui::ImGuiMenu* menu) {
	this->viewer = viewer;
	this->menu = menu;
	init();
}
void StartMenu::init() {
	viewer->plugins.push_back(menu);
	menu->callback_draw_viewer_menu = [&]()
	{
		// Draw parent menu content
		//menu->draw_viewer_menu();
		// Add new group
		main_menu();
		
	};
	menu->callback_draw_custom_window = [&]()
	{
		if (!viewer->running_game) {
			//test();
			start_menu();
			return;
		}
		if (viewer->finish_game) {
			finish_menu();
			return;
		}
		if (viewer->pause_game) {
			pause_menu();
			return;
		}
		game_menu();
		if (viewer_info)	game_info();
	};
}


void StartMenu::test() {
	int my_image_width = 0;
	int my_image_height = 0;
	GLuint my_image_texture = 0;
	bool ret = LoadTextureFromFile("img/grass.png", &my_image_texture, &my_image_width, &my_image_height);
	IM_ASSERT(ret);
	ImGui::Begin("OpenGL Texture Text");
	ImGui::Text("pointer = %p", my_image_texture);
	ImGui::Text("size = %d x %d", my_image_width, my_image_height);
	ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));
	ImGui::End();

}
void StartMenu::main_menu() {
	if (ImGui::CollapsingHeader("Game Procs", ImGuiTreeNodeFlags_OpenOnDoubleClick))
	{
		////handle sound///////
		static bool isOn = false;
		static int volume = 10;
		if (ImGui::Checkbox("Sound", &isOn))
		{
			// do something
			if (isOn)
				(((Game*)viewer->game)->setSound());
			else
				(((Game*)viewer->game)->stopSound());
			//isOn = !isOn;
		}
		if (ImGui::InputInt("Num letters", &volume))
		{
			volume = std::max(0, std::min(10, volume));
			(((Game*)viewer->game)->setVolume(volume));
		}
		/////handle textures ////////
		/////handle textures ////////
		const char* items[] = { "defaul", "grass", "sand" };
		static const char* current_item = NULL;
		if (ImGui::BeginCombo("Set Ground", current_item)) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects

				if (ImGui::Selectable(items[n], is_selected)) {
					if (strcmp(items[n], "defaul")) {
						(((Game*)viewer->game)->setGround(0));
					}
					if (strcmp(items[n], "sand")) {
						(((Game*)viewer->game)->setGround(1));
					}
					if (strcmp(items[n], "grass")) {
						(((Game*)viewer->game)->setGround(2));
					}
				}
			}
			ImGui::EndCombo();
		}
		////////////////////
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);
		ImGui::DragFloat("Shininess", &(viewer->data().shininess), 0.05f, 0.0f, 100.0f);
		ImGui::PopItemWidth();
		/////////////////
		ImGui::ColorEdit4("Background", viewer->core().background_color.data(),
			ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel);
		///////////////
		ImGui::SliderFloat("accelerate", &(
			((Game*)viewer->game)->enemy_manager->accs), 0.0f, 2.0f);
		///////////////
		ImGui::SliderFloat("Max_Speec", &(
			((Game*)viewer->game)->enemy_manager->max_speed), 0.0f, 5.0f);
		///////////////
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0, 255, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0, 40, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0, 40, 0));
		if (ImGui::Button("Game_Info")) {
			this->viewer_info = true;
		}
		ImGui::PopStyleColor(3);
	}
}
void StartMenu::start_menu() {
	int my_image_width = 0;
	int my_image_height = 0;
	GLuint my_image_texture = 0;
	bool ret = LoadTextureFromFile("img/intro.jpg", &my_image_texture, &my_image_width, &my_image_height);
	IM_ASSERT(ret);
	ImGui::SetNextWindowPos(ImVec2(180.f * menu->menu_scaling(), 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(700, 700), ImGuiCond_FirstUseEver);
	ImGui::Begin(
		"Snake Game", nullptr,
		ImGuiWindowFlags_NoSavedSettings
	);
	
	ImGui::InputText("Your Name", name, IM_ARRAYSIZE(name));
	
	if (ImGui::Button("Start Game", ImVec2(-1, 0)))
	{
		viewer->init_game();
		(((Game*)viewer->game)->init_score(name));
		Renderer* renderer = (Renderer*)glfwGetWindowUserPointer(viewer->window);
		renderer->StartGame(viewer);
		viewer->set_second_camera();
		std::cout << "Start Game\n";
	}
	if (ImGui::Button("End Game", ImVec2(-1, 0)))
	{
		viewer->end_game = true;
		std::cout << "End Game\n";
	}
	ImGui::BulletText(
		"Game Info:\n"
		"this is a final project in 3d animation\n"
		"Right Click on the enemy to catch it\n"
		"Press Start to start the game\n");
	ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));
	ImGui::End();
}
void StartMenu::pause_menu() {
	ImGui::SetNextWindowPos(ImVec2(180.f * menu->menu_scaling(), 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(700, 200), ImGuiCond_FirstUseEver);
	ImGui::Begin("Paused", nullptr, ImGuiWindowFlags_NoSavedSettings);
	if (ImGui::Button("Resume Game", ImVec2(-1, 0)))
	{
		viewer->pause_game = false;
	}
	if (ImGui::Button("End Game", ImVec2(-1, 0)))
	{
		viewer->end_game = true;
		std::cout << "End Game\n";
	}
	ImGui::End();
}
void StartMenu::finish_menu() {
	int score = (((Game*)viewer->game)->score);
	std::string old_user = (((Game*)viewer->game)->old_user_name);
	std::string user = (((Game*)viewer->game)->user_name);
	ImGui::SetNextWindowPos(ImVec2(180.f * menu->menu_scaling(), 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(700, 200), ImGuiCond_FirstUseEver);
	ImGui::Begin("Done Playing", nullptr, ImGuiWindowFlags_NoSavedSettings);
	if (score > (((Game*)viewer->game)->high_score)) {
		std::string sc = std::to_string(score);
		(((Game*)viewer->game)->finish_game(score));
		ImGui::Text("Congrats!!!! ");
		ImGui::SameLine();
		ImGui::InputText("",user);
		ImGui::Text("YOU SET A NEW HIGH SCORE!!!! ");
		ImGui::SameLine();
		ImGui::InputText("", std::to_string(score));
		ImGui::Text("WOHOO YOU JUST BEATED ");
		ImGui::SameLine();
		ImGui::InputText("", old_user);
		ImGui::SliderInt("RATING", &rating, 0, 5);
		if (ImGui::Button("Submit", ImVec2(-1, 0)) || submitted)
		{
			submitted = true;
			ImGui::SetNextWindowPos(ImVec2(600.f * menu->menu_scaling(), 10), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
			ImGui::Begin(
				"ThanKing", nullptr,
				ImGuiWindowFlags_NoSavedSettings);
			ImGui::TextColored(ImColor(255, 0, 0), "Thank You For Your Rating\n We Hope That You Had A Good Time Playing Our Game");
			ImGui::TextColored(ImColor(155, 140, 10), "It Was One Of The Best Courses That We Took\n We Enjoyed Working On The Project\n And We Wish We Had More Time For It");
			ImGui::TextColored(ImColor(0, 140, 10), "Thank Your For Your Time\n And For The Efforts You Made To Teach Us\nWe Wish You A Great Time");
			ImGui::TextColored(ImColor(0, 140, 10), "Please Submit The Final Grade ");
			ImGui::SliderInt("Grade", &grade, 0, 100);
			ImGui::End();

		}
		if (ImGui::Button("End Game", ImVec2(-1, 0)))
		{
			viewer->end_game = true;
			std::cout << "End Game\n";
		}
		ImGui::End();
		return;
	}
	
	ImGui::InputText("Your Score", std::to_string(score));
	if (ImGui::Button("End Game", ImVec2(-1, 0)))
	{
		viewer->end_game = true;
		std::cout << "End Game\n";
	}
	ImGui::End();
}
void StartMenu::game_menu() {
	int score = (((Game*)viewer->game)->score);
	int level = (((Game*)viewer->game)->level);
	static char* s = "\n";
	static char* l = "Next Level";
	int sp = (((Game*)viewer->game)->level);
	int enemies = (((Game*)viewer->game)->current_enemies);
	if (enemies <= 0) {
		s = "Press Next Level to continue\n";
	}
	else {
		s = "please finish all objects first\n";
	}

	if (sp >= 2) {
		l = "Special Level\n";
		s = "Press Special Level to continue\n";
	}
	// Define next window position + size
	ImGui::SetNextWindowPos(ImVec2(310.f * menu->menu_scaling(), 5), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
	ImGui::Begin(
		"Game Info", nullptr,
		ImGuiWindowFlags_NoSavedSettings
	);
	if (ImGui::Button(l, ImVec2(-1, 0)))
	{
		if (enemies <= 0) {
			(((Game*)viewer->game)->set_level());
			s = "\n";
			std::cout << "Next Level\n";
		}
	}
	if (ImGui::Button("Pause Game", ImVec2(-1, 0)))
	{
		viewer->pause_game = true;
		std::cout << "Pause Game\n";
	}
	if (ImGui::Button("Finish Game", ImVec2(-1, 0)))
	{
		viewer->finish_game = true;
		viewer->pause_game = true;
		std::cout << "Finish Game\n";
	}
	ImGui::InputText("Your Score", std::to_string(score));
	ImGui::InputText("Your Level", std::to_string(level));
	ImGui::BulletText(s);
	ImGui::End();
}
void StartMenu::game_info() {
	ImGui::SetNextWindowPos(ImVec2(600.f * menu->menu_scaling(), 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_FirstUseEver);
	ImGui::Begin(
		"gameInfo", nullptr,
		ImGuiWindowFlags_NoSavedSettings
	);
	static char * inf = "";
	int level = (((Game*)viewer->game)->level);
	if (level == 0) inf = "right click on object to kill";
	if (level == 1 || level == 2) inf = "hit space to see enemy\n right click on object to kill";
	if (level == 3) inf = "try to catch as many enemies as you can!!";
	ImGui::TextColored(ImColor(255, 0, 0),inf );
	if (ImGui::Button("close_Info")) {
		this->viewer_info = false;
	}
	ImGui::End();
}
bool StartMenu::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Upload pixels into texture
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}
