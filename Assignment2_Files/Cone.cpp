/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cone class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cone.h"
#include <math.h>

/**
* Cone's intersection method.  The input is a ray (pos, dir).
*/
float Cone::intersect(glm::vec3 posn, glm::vec3 dir)
{
    float a = (dir.z * dir.z) + (dir.x * dir.x) - pow(radius / height, 2) * (dir.y * dir.y);
    float b = (2 * (posn.x - center.x) * dir.x) + (2 * (posn.z - center.z) * dir.z) + (2 * (pow(radius / height, 2)) * (height - posn.y + center.y) * dir.y);
    float c = pow(posn.x - center.x, 2) + pow(posn.z - center.z, 2) - pow(radius / height, 2) * pow(height - posn.y + center.y, 2);

    float discr = b * b - (4 * a * c);
    if(fabs(discr) < 0.001) return -1.0;
    if(discr < 0.0) return -1.0;

    float t1 = (-b - sqrt(discr)) / 2 * a;
    float t2 = (-b + sqrt(discr)) / 2 * a;

    glm::vec3 t1point = glm::vec3(posn + t1 * dir);
    glm::vec3 t2point = glm::vec3(posn + t2 * dir);

    if (t1 < t2) {
        if ((center.y + height) > t1point.y && center.y < t1point.y){
            return t1;
        }
        return -1.0f;
    } else {
        if ((center.y + height) > t2point.y && center.y < t2point.y){
            return t2;
        }
        return -1.0f;
    }
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
glm::vec3 Cone::normal(glm::vec3 p)
{
    float alpha = atan((p.x - center.x) / (p.z - center.z));
    float theta = atan(radius / height);
    glm::vec3 n = glm::vec3(sin(alpha)*cos(theta), sin(theta), cos(alpha)*cos(theta));
    return n;
}
