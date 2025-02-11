/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Plane class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Plane.h"
#include <math.h>


/**
* Checks if a point pt is inside the current polygon
* Implement a point inclusion test using
* member variables a, b, c, d.
*/
bool Plane::isInside(glm::vec3 pt)
{

    //=== Complete this function ====
    glm::vec3 n = normal(pt);

    glm::vec3 ua = b-a; glm::vec3 va = pt-a;
    glm::vec3 ub = c-b; glm::vec3 vb = pt-b;
    glm::vec3 uc = d-c; glm::vec3 vc = pt-c;
    glm::vec3 ud = a-d; glm::vec3 vd = pt-d;

    bool test1 = glm::dot(glm::cross(ua, va), n) > 0;
    bool test2 = glm::dot(glm::cross(ub, vb), n) > 0;
    bool test3 = glm::dot(glm::cross(uc, vc), n) > 0;
    bool test4 = glm::dot(glm::cross(ud, vd), n) > 0;


    if (test1 && test2 && test3 && test4) {
        return true;
    } else {
        return false;
    }
}

/**
* Plane's intersection method.  The input is a ray (pos, dir).
*/
float Plane::intersect(glm::vec3 posn, glm::vec3 dir)
{
    glm::vec3 n = normal(posn);
    glm::vec3 vdif = a - posn;
    float vdotn = glm::dot(dir, n);
    if(fabs(vdotn) < 1.e-4) return -1;
    float t = glm::dot(vdif, n)/vdotn;
    if(fabs(t) < 0.0001) return -1;
    glm::vec3 q = posn + dir*t;
    if(isInside(q)) return t;
    else return -1;
}

/**
* Returns the unit normal vector at a given point.
* Compute the plane's normal vector using
* member variables a, b, c, d.
* The parameter pt is a dummy variable and is not used.
*/
glm::vec3 Plane::normal(glm::vec3 pt)
{
    glm::vec3 n = glm::vec3(0);

    //=== Complete this function ====
    n = glm::normalize(glm::cross((b - a), (d - a)));

    return n;
}
