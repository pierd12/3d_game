# EngineForAnimationCourse
Graphic Engine based on Libigl


Made By :
Pier Damouny
Adham Jbarin

To compile the project :
1.Clone or download the project
2.Change EngineForAnimationCourse\CmakeList File (Use ImGui -- On) (Use PNG -- On)
3.Change EngineForAnimationCourse\tutorial\sandBox\CmakeList File
3.1 Add igl::opengl_glfw_imgui igl::png to target_link_libraries
4.Run Cmake gui. choose the project folder and destination folder for the cpp project files. choose after pressing configure choose compiler (VS2019 for example). After finish configuration successfully, press configure again and after it finishes press generate. 
5.If everything pass successfully got to the destination folder and launch the project. 
6.download our files from submission system \ github
7.Change EngineForAnimationCourse\igl\opengl\glfw\*.h|.cpp files
7.1[Display | renderer | Viewer]
8.Change EngineForAnimationCourse\igl\opengl\*.h|.cpp files
8.1[Movable| ViewerData ]
9.Change EngineForAnimationCourse\tutorial\sandBox\[main.cpp | inputManager.cpp]
10.Add to EngineForAnimationCourse\tutorial\tutorial\sandBox\
10.1 the [img | include | sounds ] folders
10.2 the [ikpMP3.dll | irrKlang.dll | irrKlang.lib | level1 | configuration | Score ] files
10.3 the [CollisionManager | EnemyManager | Game | Snake | SoundManager | StartMenu | SurroundingManager ] classes
11.Set sandBox as a startup project and compile the project (it could take few mineutes);


Project Detail:
---a menu for gameProc wich include 
----sound
----volume
----setGround(using Png)
----Shinieness   change core brightness
----Background to change the color
----accelerate to change to acceleration speed when bouncing
----Max Speed of the moving mesh
----game info show info of current level
---a menu bar for Game Info 
----next level to go to next level
----pause game stop rendering (pop up new bar for endgame or resume)
----finish game to finish the game (pop up new bar for rating and set high record)
----Current Score
----Current Level
----text if should go next level
--- the renderer window 
----snake of 11 cylinder in  x y z = 0 0 0
----first level non moving meshes 
----second level meshesh falling in gravity accelaration by hitting space
----third level 4 spheres in a cubek envirment bouncing depends on your max speed and accs
Project Implementaion:
--load the game init every thing
--detect using glfwmousepressed
--use Ik to reach it in snake
--detect if reached using Detect Collosion in CollisionManager
--sound using irrklang soundeffect2d \ set volume \ stop in SoundManager
--set ground using readPnd \ set texture in SorroundingManager
--Handle enemies and level in EnemyManager
--Handle the gameProc and game Loop in Game
--Handle menu bars in Start Menu

Our THOUGHTS:
--it was very amusing working on the project 
--libigl is a good library for 3d rendering and is fast 
--imgui is very powerful and intersting 
--hard time:
---adding imgui was a little bit hard since our renderer and viewer and display and input manager all are in
----the viewer in libigl 
---we didnt know how to add fog since we use glad.h wich dont have GLFOGV F FV 
---Cube Maps is not implemented in libigl 
---exam period and other projects 
---we wish we had more time to do more and more things in that project
