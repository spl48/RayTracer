/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cone class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#ifndef H_CONE
#define H_CONE
#include <glm/glm.hpp>
#include "SceneObject.h"

/**
 * Defines a simple Cylinder located at 'center'
 * with the specified radius and height
 */
class Cone : public SceneObject
{

private:
    glm::vec3 center;
    float radius;
    float height;

public:
    Cone()
        : center(glm::vec3(0)), radius(1), height(1)  //Default constructor creates a unit Cone
    {
        color = glm::vec3(1);
    };

    Cone(glm::vec3 c, float r, float h, glm::vec3 col)
        : center(c), radius(r), height(h)
    {
        color = col;
    };

    float intersect(glm::vec3 posn, glm::vec3 dir);

    glm::vec3 normal(glm::vec3 p);

};

#endif //!H_Cone
