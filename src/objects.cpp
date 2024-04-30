#include "game.h"

/* Object::Object(){ */
/* } */


/****************************************************/
/*                   USER                          */

/* User::User(){ */
/* } */


/****************************************************/
/*                   PLANET                        */

Planet::Planet(float vertices[], int indices[]) : shader(src_vertex, src_fragment){
   vtx.create_VBO(vertices, sizeof(*vertices));
   vtx.create_EBO(indices, sizeof(*indices));

}
