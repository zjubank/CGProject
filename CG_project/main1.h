//
//  main1.h
//  CG_project
//
//  Created by lhq on 15/6/22.
//  Copyright (c) 2015年 lhq. All rights reserved.
//

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "glm.h"
//#include "glm.hpp"
//#include <stdlib.h>
#include <glut/glut.h>
#include <opengl/gl.h>
#define GL_BGR 0x80E0
//#define M_PI 3.141592653589793238
#define GL_LIGHT_MODEL_COLOR_CONTROL_EXT  0x81F8
#define GL_SEPARATE_SPECULAR_COLOR_EXT    0x81FA
#define cc_x 0.4
#define PI 3.14159265358979323846
using namespace std;
// roam setting
static float c = M_PI / 180.0f; //»¡¶ÈºÍ½Ç¶È×ª»»²ÎÊý
GLint displayFPS = 0;
static int du = 90;
static int oldmy = -1;
static int oldmx = -1; //duÊÇÊÓµãÈÆyÖáµÄ½Ç¶È,openglÀïÄ¬ÈÏyÖáÊÇÉÏ·½Ïò
static float r = 1.5f; // distance between viewport and y-axis
static float h = 0.0f; // coordinate at y-axis
int angle = 90;

// display setting
GLdouble scale = 1.5;
GLfloat screenw = 100;
GLfloat screenh = 100;
const int screenWidth = 800;
const int screenHeight = 800;
unsigned int textureVideo[100];
unsigned int texture_black;
unsigned int textureNURBS[10];

GLfloat eye[3] = { 0, 0, 1.5f };
GLfloat center[3] = { 0, 0, 0 };
GLfloat dir[3] = { 0, 0, 1.5f };
GLfloat eyeVar = 1.0f;

// BMP file type supporting
#define BITMAP_ID 0x4D42

typedef unsigned char byte;
typedef unsigned short U16;
typedef unsigned int   U32;
typedef struct tagBITMAPFILEHEADER
{
    U16 bfType;
    U32 bfSize;
    U16 bfReserved1;
    U16 bfReserved2;
    U32 bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    U32 biSize;
    U32 biWidth;
    U32 biHeight;
    U16 biPlanes;
    U16 biBitCount;
    U32 biCompression;
    U32 biSizeImage;
    U32 biXPelsPerMeter;
    U32 biYPelsPerMeter;
    U32 biClrUsed;
    U32 biClrImportant;
} BITMAPINFOHEADER;

// NURBS Texture Setting: Mandelbrot Set
const int TEXW = 512;
const int TEXH = 512;
static GLubyte checkImage[TEXW][TEXH][3];
float centerRe = -0.5;
float centerIm = 0.0;
float radius = 1.0;

// NURBS Draw Setting
GLUnurbsObj *theNurb;
static GLfloat ctlPoints[8][8][3];
static GLfloat tcoords[2][2][2] = { { { 0, 0 }, { 0, 1 } }, { { 1, 0 }, { 1, 1 } } };

// NURBS Trim Setting
static GLfloat edgePt[5][2] =
{ { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0 } };
static const int nbkntcu = 8;
static GLfloat curveKnots[nbkntcu] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };
static GLfloat curvePt[4][2] =
{ { 0.2, 0.25 }, { 0.2, 0.45 }, { 0.45, 0.45 }, { 0.45, 0.25 } };
static const int nbkpt = 6;
static GLfloat pwlPt[nbkpt][2] =
{ { 0.45, 0.25 }, { 0.43, 0.20 }, { 0.35, 0.15 }, { 0.30, 0.15 }, { 0.22, 0.20 }, { 0.2, 0.25 } };

// collision test
class protectedBox {
private:
    float xMin, xMax;
    float yMin, yMax;
    float zMin, zMax;
public:
    protectedBox();
    
    void putCoordinate(float xMinTmp, float xMaxTmp, float yMinTmp, float yMaxTmp, float zMinTmp, float zMaxTmp);
    
    bool isLegal(float x, float y, float z);
    
    ~protectedBox() {}
};

// Object drawing and editible lighting
const int objNum = 9;
char objFileName[objNum][32] = { "model/floor.obj", "model/ball1.obj", "model/cube.obj", "model/column.obj", "model/circularColumn.obj",
    "model/pyramid.obj", "model/circularPyramid.obj", "model/table.obj", "model/circularTable.obj"};
bool cc_act = false;
bool cc_color = true;
float cc_none[] = { 0 * cc_x, 0 * cc_x, 0 * cc_x, 1 };
float cc_cyan[] = { 0 * cc_x, 1 * cc_x, 1 * cc_x, 0 };
float cc_golden[] = { 1 * cc_x, 215.0 / 255 * cc_x, 0, 1 };
float cc_shininess = 20;
float cc_white[] = { 1 * cc_x, 1 * cc_x, 1 * cc_x, 1 };
//the orginal material
float cc_orginal_amb[] = { 0.2, 0.2, 0.2, 1 };
float cc_orginal_dif[] = { 0.8, 0.8, 0.8, 1 };
float cc_orginal_spe[] = { 0, 0, 0, 1 };
GLfloat lightPos[] = { 0, 5, 10 }, lightPosVar = 1.0f;
GLfloat lightColor[] = { 1, 1, 1 }, lightColorVar = 0.1f;
int operation = 0; //0:lightPos  1:lightColor  2:eye
GLfloat cc_eye[] = { 0, 0, 10 };
GLfloat cc_center[] = { 0, 0, 0 };
GLfloat cc_up[] = { 0, 1, 0 };
GLfloat cc_eyeVar = 1.0f;

GLfloat cc_lightPos[] = { 0, 5, 10 }, cc_lightPosVar = 1.0f;
GLfloat cc_lightColor[] = { 1, 1, 1 }, cc_lightColorVar = 0.1f;
GLfloat fRotate = 0;
float cc_Rotate = 180;
GLint list = 0;

int textureObjectCnt = 0;
GLuint textureObjects[400];
char texFileName[400][50];

typedef struct vertex{
    GLfloat pos[3];
}vertex;
vector<vertex>v;

typedef struct vertexTexture{
    GLfloat pos[2];
}vertexTexture;
vector<vertexTexture>vt;

const int objectCnt = 50;
static int objectNow;
protectedBox protectedBoxs[objectCnt];
bool setProtectedBoxNURBS = false;

// generate mandelbrot set evolution process
static GLuint textureMandelbrot[70];
const int mandelbrotSize = 256;
static GLubyte mandelbrotImage[mandelbrotSize][mandelbrotSize][3];

bool snapScreen(int width, int height, const char *file);

// load textures
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);
void loadTexture(int i, char* filename, bool type);

// load and play the video
void initTextureVideo();
void renderVideoFrame(GLfloat x);
void renderVideo();
void getVideoFrame(int value);

// load and draw objects with textures
const int MAXLENGTH = 1000;
void loadObjectTextures();
vertex normal(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3]);
void drawObj(int cur);
GLint generateList();

// generate and display the mandelbrot set evolution process
void initMandelbrotVideo();
void makeMandelbrotImage(int iterationCnt);
void renderMandelbrotVideo();
int getMandelbrot(float C_re, float C_im, int max_count);

// to deal with keyboard and mouse event
void Mouse(int button, int state, int x, int y);
void onMouseMove(int x, int y);
void key(unsigned char k, int x, int y);

// global initializing
void initGlobalScene();

// display the scene
void display();
void reshape(int w, int h);
void idle();

void cc_Draw_Element();

// draw the NURBS
void trimSurfaceNURBS();
void initSurfaceNURBS();
void drawFrameOfNURBS();
void drawSurfaceNURBS(bool trim);
void genTextureNURBS(int maxs, int maxt);
void initTextureNURBS();

float min(float x, float y) { return x < y ? x : y; }
float max(float x, float y) { return x > y ? x : y; }

