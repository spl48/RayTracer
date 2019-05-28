/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cylinder class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cylinder.h"
#include <math.h>

/**
* Cylinder's intersection method.  The input is a ray (pos, dir).
*/
float Cylinder::intersect(glm::vec3 posn, glm::vec3 dir)
{
    float a = (dir.x * dir.x) + (dir.z * dir.z);
    float b = 2 * (dir.x * (posn.x - center.x) + dir.z * (posn.z - center.z));
    float c = (pow((posn.x - center.x), 2) + pow((posn.z - center.z), 2) - (radius * radius));
    float discr = b * b - (4 * a * c);
    if(fabs(discr) < 0.001) return -1.0;
    if(discr < 0.0) return -1.0;

    float t1 = (-b - sqrt(discr)) / 2 * a;
    float t2 = (-b + sqrt(discr)) / 2 * a;

    glm::vec3 t1point = glm::vec3(posn + t1 * dir);
    glm::vec3 t2point = glm::vec3(posn + t2 * dir);

    if (t1point.y > (height + center.y) && center.y < t1point.y) {
        if (t2point.y <= (height + center.y)) return t2;
        else t1 = -1.0;
    }
    if(fabs(t2) < 0.001 ) t2 = -1.0;
//    if (t2point.y > (height + center.y)) {
//        if (t1point.y <= (height + center.y)) return t1;
//        else t2 = -1.0;
//    }

    return (t1 < t2)? t1: t2;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
glm::vec3 Cylinder::normal(glm::vec3 p)
{
//    glm::vec3 n;
//    if (p.y >= center.y + height) {
//        glm::vec3 n = glm::vec3(0, 1, 0);
//    } else {
//        glm::vec3 n = glm::vec3((p.x-center.x)/radius, 0, (p.z-center.z)/radius);
//    }
    glm::vec3 n = glm::vec3((p.x-center.x)/radius, 0, (p.z-center.z)/radius);
    n = glm::normalize(n);
    return n;
}
