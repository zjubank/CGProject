//
//  mtl.h
//  CGProject
//
//  Created by 尹航 on 16/1/9.
//  Copyright © 2016年 zjubank. All rights reserved.
//

#ifndef MTL_H
#define MTL_H

// C++ Headers
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
// C Headers
#include <stdio.h>
// OpenGL and GLM Headers
#ifndef __linux
#include <glut/glut.h>
#include <opengl/gl.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif
//#include "glm.hpp"

using namespace std;

class Mtl
{
public:
    bool loadmtl( string filepath );
};

#endif /* MTL_H */
