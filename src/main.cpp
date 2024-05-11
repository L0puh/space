#include "game.h" 
#include "utils.h"
#include <GLFW/glfw3.h>
#include <cstdlib>

#define DEBUG_MODE

float get_deltatime(float *last_time){
   float time, deltatime;
   time = glfwGetTime();
   deltatime = time - *last_time;
   *last_time= time;
   return deltatime;
}

int main() {
   const float width = 400, height = 400;
   GLFWwindow *window = init_window(width, height);
  
   // transparent bg of textures
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef DEBUG_MODE
   utils::init_debug_console(window);
   glEnable(GL_DEBUG_OUTPUT);
   glDebugMessageCallback(message_callback, 0);
   utils::log("DEBUG MODE");
#endif 

   User user("../shaders/user.vert", "../shaders/user.frag", "../textures/spaceship.png", PNG);
   Object bg_obj("../shaders/user.vert", "../shaders/user.frag", "../textures/bg.jpg", JPG);
   Object star("../shaders/standard.vert", "../shaders/standard.frag");
   Camera camera;
   
   float rand_points_x[320];
   for (int i=0; i!=320;i++){
      rand_points_x[i] = std::rand()*0.0000000001;
      printf("%f\n", rand_points_x[i]);
   }
   float rand_points_y[320];
   for (int i=0; i!=320;i++){
      rand_points_y[i] = std::rand()*0.0000000001;
      printf("%f\n", rand_points_y[i]);
   }


   float last_frame = 0.0f, deltatime;
   while (!glfwWindowShouldClose(window)){
      deltatime = get_deltatime(&last_frame); 
      glm::mat4 model, model_bg, model_star;
      model = model_bg = glm::mat4(1.0f);
      glfwSetKeyCallback(window, Input::key_callback);
      camera.get_movement(window, deltatime); 
      camera.update();
    
      bg_obj.scale_object(window, &model_bg, glm::vec2(10.0f, 10.0f));
      user.translate_object(window, &model, camera.pos);
      user.rotate_object(window, &model, camera.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
      user.scale_object(window, &model, glm::vec2(0.3f, 0.3f));

      utils::debug_new_frame();
      utils::debug_console(window, &user, &camera);

      float x=0, y=0; 
      glClearBufferfv(GL_COLOR, 0, bg);
      bg_obj.draw(window, model_bg, camera.view);
      for (int i = 0; i != 320; i++){
         star.translate_object(window, &model_star, glm::vec2(rand_points_x[i], rand_points_y[i]));
         star.draw(window, model_star, camera.view);
      }
      user.draw(window, model, camera.view);
      utils::debug_console_render();
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   shut_down(window);
   return 0;
}
