#include "3D.h"
#include "game.h"
#include "state.h"
#include "utils.h"

void run_3d_prototype() {
   Texture tex_sheet("../textures/wall.jpg", JPG);
   float last_frame = 0.0f, deltatime;
   Camera_3D camera;
   Object cube("../shaders/user.vert", "../shaders/user.frag", &tex_sheet, {0, 0}, CUBE);
   
   global_states.camera_3d = &camera;
   tex_sheet.load_texture();

   glEnable(GL_DEPTH_TEST);
   glfwSetInputMode(global_states.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   while (!glfwWindowShouldClose(global_states.window)){
      deltatime = get_deltatime(&last_frame); 
      glfwSetKeyCallback(global_states.window, Input::key_callback);
      glfwSetCursorPosCallback(global_states.window, Input::mouse_callback);
      glfwSetScrollCallback(global_states.window, Input::scroll_callback);

      glClearBufferfv(GL_COLOR, 0, bg_color);
      glClear(GL_DEPTH_BUFFER_BIT);
      utils::debug_new_frame();
      camera.get_movement(deltatime);

      //drawing: 
      cube.update();
      cube.translate_object({0.0, 0.0, -10.0f});
      cube.draw(cube.model, camera.get_view(), camera.get_projection());

      utils::debug_console_render();
      glfwSwapBuffers(global_states.window);
      glfwPollEvents();
      global_states.window = global_states.window;
      global_states.w_size = get_window_size(global_states.window);
      global_states.deltatime = deltatime;
      global_states.camera_3d = &camera;
   }
}
