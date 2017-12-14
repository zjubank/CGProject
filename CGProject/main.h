//
//  main.h
//  CGProject
//
//  Created by 尹航 on 16/1/12.
//  Copyright © 2016年 zjubank. All rights reserved.
//

#ifndef main_h
#define main_h


#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <memory.h>
#include "glm.h"
#ifndef __linux
#include <glut/glut.h>
#include <opengl/gl.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif
#define GL_BGR 0x80E0
//#define M_PI 3.141592653589793238
#define GL_LIGHT_MODEL_COLOR_CONTROL_EXT  0x81F8
#define GL_SEPARATE_SPECULAR_COLOR_EXT    0x81FA
#define cc_x 0.4
#define PI 3.14159265358979323846
using namespace std;
// roam setting
static float c = M_PI / 180.0f;
GLint displayFPS = 0;
static int du = 90;
static int oldmy = -1;
static int oldmx = -1;
static float r = 1.5f;
static float h = 0.0f;
int angle = 90;

// display setting
GLdouble scale = 1.5;
GLfloat screenw = 100;
GLfloat screenh = 100;
const int screenWidth = 800;
const int screenHeight = 800;
unsigned int textureVideo[30];
unsigned int texture_black;

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

int operation = 1; //0:lightPos  1:lightColor
GLfloat FP_eye[] = { 0, 0, 10 };
GLfloat FP_center[] = { 0, 0, 0 };
GLfloat FP_up[] = { 0, 1, 0 };
GLfloat FP_eyeVar = 1.0f;

GLfloat fRotate = 0;
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


float min(float x, float y) { return x < y ? x : y; }
float max(float x, float y) { return x > y ? x : y; }

GLMmodel *stage2M;
GLMmodel *kotori2M;
unsigned int texture[101];

bool ifmainlight = true;
bool ifflashlight = false;
bool ifspotlight = false;

//灯
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
int light_color_index = 0;
GLfloat light_color[][10] = { \
    //Kotori White
    {1.0f, 1.0f ,1.0f, 1.0f},\
    //Hono Orange
    {1.0f, 0.5f, 0.0f, 1.0f},\
    //Umi Blue
    {0.0f, 0.0f, 1.0f, 1.0f},\
    //Maki Red
    {1.0f, 0.0f, 0.0f, 1.0f},\
    //Rin Yellow
    {1.0f, 1.0f, 0.0f, 1.0f},\
    //Hanayo Green
    {0.0f, 1.0f, 0.0f, 1.0f},\
    //Eli Cyan
    {0.0f, 1.0f, 1.0f, 1.0f},\
    //Nozomi Violet
    {0.5f, 0.0f, 0.5f, 1.0f},\
    //Nico Pink
    {1.0f, 0.0f, 0.5f, 1.0f} \
};

//大灯
GLfloat MainLightPos[] = { 0, 5, 10 }, MainLightPosVar = 1.0f;
GLfloat MainLightColor[] = { 1, 1, 1 }, MainLightColorVar = 0.1f;
//GLfloat light_pos[] = {5, 5, 5, 1};

//手电筒
GLfloat FLambient[] = {1, 1, 1, 1};
GLfloat FLposition[] = {eye[0], eye[1], eye[2], 1};
GLfloat FLspotangle = 30.0f;
GLfloat FLlightDir[] = {-dir[0], -dir[1], -dir[2]};


GLfloat fdir[]={0,0,-1,1};

//聚光灯
GLfloat SPambient[] = {1, 1, 1, 1};
GLfloat SPposition[] = {0, 30, 0, 1};
GLfloat SPspotangle = 30.0f;
GLfloat SPlightDir[] = {0, -1, 0, 1};



int Pointer_Count = 0;
int Time = 0;

GLint nNumPoints = 4; // 4 X 4

//                 u  v  (x,y,z)
GLfloat ctrlPoints[4][4][3] = { { { -6.0f, -6.0f, 0.0f },    // u = 0,    v = 0
    { -6.0f, -2.0f, 0.0f },    //            v = 1
    { -6.0f,  2.0f, 0.0f },    //            v = 2
    { -6.0f,  6.0f, 0.0f } }, //            v = 3
    
    { { -2.0f, -6.0f, 0.0f },    // u = 1    v = 0
        { -2.0f, -2.0f, 8.0f },    //            v = 1
        { -2.0f,  2.0f, 8.0f },    //            v = 2
        { -2.0f,  6.0f, 0.0f } },    //            v = 3
    
    { { 2.0f, -6.0f, 0.0f }, // u =2        v = 0
        { 2.0f, -2.0f, 8.0f }, //            v = 1
        { 2.0f,  2.0f, 8.0f },    //            v = 2
        { 2.0f,  6.0f, 0.0f } },//            v = 3
    
    { { 6.0f, -6.0f, 0.0f },    // u = 3    v = 0
        { 6.0f, -2.0f, 0.0f },    //            v = 1
        { 6.0f,  2.0f, 0.0f },    //            v = 2
        { 6.0f,  6.0f, 0.0f } } };//            v = 3

GLfloat Knots[8] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f };
GLUnurbsObj* pNurb=NULL;

typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned int	DWORD;
typedef int LONG;


#pragma pack(2)
struct BMPfileHeader{
    WORD	bfType;
    DWORD	bfSize;
    WORD	bfReserved1;
    WORD	bfReserved2;
    DWORD	bfOffBits;
};

struct BMPinfoHeader{
    DWORD	biSize;
    LONG	biWidth;
    LONG	biHeight;
    WORD	biPlanes;
    WORD	biBitCount;
    DWORD	biCompression;
    DWORD	biSizeImage;
    LONG	biXPelsPerMeter;
    LONG	biYPelsPerMeter;
    DWORD	biClrUsed;
    DWORD	biClrImportant;
};

struct BMPpalette{
    BYTE	rgbRed;
    BYTE	rgbGreen;
    BYTE	rgbBlue;
    BYTE	rgbReserved;
};

struct BMP{
    BMPfileHeader fileHeader;
    BMPinfoHeader infoHeader;
    BMPpalette *palette;
    unsigned char *data;
};
#define BITMAP_ID 0x4D42
#define n_bmp 16

BYTE tex[64][64][3];
fstream myfin, myfout;

struct forbidden
{
    double x1;
    double z1;
    double x2;
    double z2;
};

int forbiddencnt = 4;
forbidden forbiddenlist[10] = {
    {-12, 7.5, -16, 3.5},
    {-12, 14, -16, 10.5},
    {16, 7.5, 12, 3.5},
    {16, 14, 12, 10.5},
    {10, 19, -10, 11.5}
};
// 舞台门口-17, 0, -16.5。舞台后（不精确）13, 9

float stagemtr[] = {0.5, 0.5, 0.5, 1};
GLfloat no_mat[]={0.0,0.0,0.0,1.0};
GLfloat mat_ambient[]={0.7,0.7,0.7,1.0};
GLfloat mat_ambient_color[]={0.8,0.8,0.2,1.0};
GLfloat mat_diffuse[]={0.1,0.5,0.8,1.0};
GLfloat mat_specular[]={1.0,1.0,1.0,1.0};
GLfloat no_shininess[]={0.0};
GLfloat low_shininess[]={5.0};
GLfloat high_shininess[]={100.0};
GLfloat mat_emission[]={0.3,0.2,0.2,0.0};

bool ifmodelshow = false;
bool ifmodelscale = false;
bool ifmodeltranslate = false;
bool ifmodelrotate = false;

float modelscale = 1.0;
float modeltranslate = 0.0;
float modelrotate = 0.0;

bool inroom = true;

int ViewMode = 2;

bool ifdooropen = false;
#endif /* main_h */
