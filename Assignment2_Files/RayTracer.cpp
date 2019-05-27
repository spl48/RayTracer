/*========================================================================
* COSC 363  Computer Graphics (2018)
* Ray tracer
* See Lab07.pdf for details.
*=========================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Plane.h"
#include "Ray.h"
#include "TextureBMP.h"
#include <GL/glut.h>
using namespace std;
TextureBMP floorTexture;
TextureBMP earthTexture;

const float WIDTH = 20.0;
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

vector<SceneObject*> sceneObjects;  //A global list containing pointers to objects in the scene

void drawCube (int cubex, int cubey, int cubez, int cubeSize, float rotationAngle)
{
    float radAngle = rotationAngle * 3.1415 / 180.f;
    glm::mat4 im, am, bm, cm;
    im = glm::mat4(1.0f);

    glm::vec4 pt1(0, 0, cubeSize, 1);
    glm::vec4 pt2(cubeSize, 0, cubeSize, 1);
    glm::vec4 pt3(cubeSize, 0, 0, 1);
    glm::vec4 pt4(0, 0, 0, 1);
    glm::vec4 pt5(cubeSize, cubeSize, 0, 1);
    glm::vec4 pt6(cubeSize, cubeSize, cubeSize, 1);
    glm::vec4 pt7(0, cubeSize, 0, 1);
    glm::vec4 pt8(0, cubeSize, cubeSize, 1);

    bm = glm::translate(im, glm::vec3(cubex, cubey, cubez));
    cm = glm::rotate(bm, radAngle,  glm::vec3(0.f, 1.0f, 0.f));


    pt1 = cm*pt1; pt2 = cm*pt2; pt3 = cm*pt3; pt4 = cm*pt4; pt5 = cm*pt5; pt6 = cm*pt6; pt7 = cm*pt7; pt8 = cm*pt8;

    Plane *cubePlane1 = new Plane (pt1, pt2, pt3, pt4,
                                   glm::vec3(0, 0.5, 0.0)); //Colour

    Plane *cubePlane2 = new Plane (pt2, pt3, pt5, pt6,
                                   glm::vec3(0, 0.5, 0.0)); //Colour

    Plane *cubePlane3 = new Plane (pt1, pt4, pt7, pt8,
                                   glm::vec3(0, 0.5, 0.0)); //Colour

    Plane *cubePlane4 = new Plane (pt1, pt2, pt6, pt8,
                                   glm::vec3(0, 0.5, 0.0)); //Colour

    Plane *cubePlane5 = new Plane (pt4, pt3, pt5, pt7,
                                   glm::vec3(0, 0.5, 0.0)); //Colour

    Plane *cubePlane6 = new Plane (pt8, pt6, pt5, pt7,
                                   glm::vec3(0, 0.5, 0.0)); //Colour

    sceneObjects.push_back(cubePlane1);
    sceneObjects.push_back(cubePlane2);
    sceneObjects.push_back(cubePlane3);
    sceneObjects.push_back(cubePlane4);
    sceneObjects.push_back(cubePlane5);
    sceneObjects.push_back(cubePlane6);
}


//---The most important function in a ray tracer! ----------------------------------
//   Computes the colour value obtained by tracing a ray and finding its
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
    glm::vec3 backgroundCol(0);
    glm::vec3 light(10, 40, -3);
    glm::vec3 ambientCol(0.2);   //Ambient color of light

    ray.closestPt(sceneObjects);		//Compute the closest point of intersetion of objects with the ray

    if(ray.xindex == -1) return backgroundCol;      //If there is no intersection return background colour

    glm::vec3 materialCol = sceneObjects[ray.xindex]->getColor(); //else return object's colour

    glm::vec3 normalVector = sceneObjects[ray.xindex]->normal(ray.xpt);
    glm::vec3 lightVector = light - ray.xpt;
    lightVector = glm::normalize(lightVector);

    Ray shadow(ray.xpt, lightVector);
    shadow.closestPt(sceneObjects);

    glm::vec3 colorSum;

    glm::vec3 reflVector = glm::reflect(-lightVector, normalVector);
    float spec = glm::dot(reflVector, normalVector);
    spec = pow(spec, 10);
    if (spec < 0) {
        spec = 0;
    } else {
        spec *= (1, 1, 1);
    }

    float lDotn = glm::dot(lightVector, normalVector);

    if (lDotn <= 0 || (shadow.xindex > -1 && shadow.xdist < length(light - ray.xpt))) {
        colorSum = ambientCol * materialCol;
    } else {
        colorSum = ambientCol * materialCol + lDotn * materialCol + spec;
    }

    if(ray.xindex == 0 && step < MAX_STEPS)
    {
        glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVector);
        Ray reflectedRay(ray.xpt, reflectedDir);
        glm::vec3 reflectedCol = trace(reflectedRay, step+1); //Recursion!
        colorSum = colorSum + (0.8f*reflectedCol);
    }

    if(ray.xindex == 1)
    {
        float a1 = -40; float a2 = -300; float b1 = -30; float b2 = 30;
        float texcoords = (ray.xpt.z - a1)/(a2-a1);
        float texcoordt = (ray.xpt.x - b1)/(b2-b1);
        colorSum = floorTexture.getColorAt(texcoords, texcoordt);
        if (lDotn <= 0 || (shadow.xindex > -1 && shadow.xdist < length(light - ray.xpt))) {
            colorSum = ambientCol * colorSum;
        } else {
            colorSum = ambientCol * colorSum + lDotn * colorSum + spec;
        }
    }

    if(ray.xindex == 2 && step < MAX_STEPS)
    {
        float eta = 1/1.1f;
        glm::vec3 g = glm::refract(ray.dir, normalVector, eta);
        Ray refRay(ray.xpt, g);
        refRay.closestPt(sceneObjects);
        if(refRay.xindex == -1) return colorSum;
        glm::vec3 m = sceneObjects[refRay.xindex]->normal(refRay.xpt);
        glm::vec3 h = glm::refract(g, -m, 1.0f/eta);

        Ray refRay2(ray.xpt, h);
        glm::vec3 refCol = trace(refRay2, step+1); //Recursion!
        colorSum += refCol;
    }

    if(ray.xindex == 3 && step < MAX_STEPS)
    {
        glm::vec3 h = ray.dir;
        Ray refRay(ray.xpt, h);
        refRay.closestPt(sceneObjects);
        if(refRay.xindex == -1) return colorSum;


        Ray refRay2(refRay.xpt, h);
        glm::vec3 refCol = trace(refRay2, step+1); //Recursion!
        colorSum += refCol;
    }

    if(ray.xindex == 4)
    {
        float a1 = 20; float a2 = 40; float b1 = 20; float b2 = 40;
        float texcoordt = (ray.xpt.y - a1)/(a2-a1);
        float texcoords = (ray.xpt.x - b1)/(b2-b1);
        colorSum = earthTexture.getColorAt(texcoords, texcoordt);
        if (lDotn <= 0 || (shadow.xindex > -1 && shadow.xdist < length(light - ray.xpt))) {
            colorSum = ambientCol * colorSum;
        } else {
            colorSum = ambientCol * colorSum + lDotn * colorSum + spec;
        }
    }

    return colorSum;

}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
    float xp, yp;  //grid point
    float cellX = (XMAX-XMIN)/NUMDIV;  //cell width
    float cellY = (YMAX-YMIN)/NUMDIV;  //cell height

    glm::vec3 eye(0., 0., 0.);  //The eye position (source of primary rays) is the origin

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);  //Each cell is a quad.

    for(int i = 0; i < NUMDIV; i++)  	//For each grid point xp, yp
    {
        xp = XMIN + i*cellX;
        for(int j = 0; j < NUMDIV; j++)
        {
            yp = YMIN + j*cellY;

            glm::vec3 dir(xp+0.5*cellX, yp+0.5*cellY, -EDIST);	//direction of the primary ray

            Ray ray = Ray(eye, dir);		//Create a ray originating from the camera in the direction 'dir'
            ray.normalize();				//Normalize the direction of the ray to a unit vector
            glm::vec3 col = trace (ray, 1); //Trace the primary ray and get the colour value

            glColor3f(col.r, col.g, col.b);
            glVertex2f(xp, yp);				//Draw each cell with its color value
            glVertex2f(xp+cellX, yp);
            glVertex2f(xp+cellX, yp+cellY);
            glVertex2f(xp, yp+cellY);
        }
    }

    glEnd();
    glFlush();
}


//---This function initializes the scene -------------------------------------------
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    glClearColor(0, 0, 0, 1);

    floorTexture = TextureBMP("floor.bmp");
    earthTexture = TextureBMP("Earth.bmp");

    //-- Create a pointer to a sphere object
    Sphere *sphere1 = new Sphere(glm::vec3(-5.0, 0.0, -120.0), 10.0, glm::vec3(0, 0, 1));
    sceneObjects.push_back(sphere1);

    Plane *plane = new Plane (glm::vec3(-30., -20, -40),  //Point A
                              glm::vec3(30., -20, -40),  //Point B
                              glm::vec3(30., -20, -300),  //Point C
                              glm::vec3(-30., -20, -300),  //Point D
                                glm::vec3(0.5, 0.5, 0));  //Colour

    sceneObjects.push_back(plane);

    Sphere *tranlucentSphere = new Sphere(glm::vec3(-7.0, -5.0, -110.0), 4.0, glm::vec3(0.3, 0.3, 0.3));
    sceneObjects.push_back(tranlucentSphere);

    Sphere *transparentSphere = new Sphere(glm::vec3(10.0, -12.0, -110.0), 4.0, glm::vec3(0.7, 0.3, 0.3));
    sceneObjects.push_back(transparentSphere);

    Sphere *texturedSphere = new Sphere(glm::vec3(30.0, 30.0, -170.0), 10.0, glm::vec3(0.0, 0.0, 0.0));
    sceneObjects.push_back(texturedSphere);

    //-- Cube made of six planes
    drawCube(10, -20, -140, 8, 30);
}



int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracer");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
