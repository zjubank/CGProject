//
//  obj.h
//  CGProject
//
//  Created by 尹航 on 16/1/9.
//  Copyright © 2016年 zjubank. All rights reserved.
//

#ifndef OBJ_H
#define OBJ_H

// C++ Headers
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
// C Headers
#include <stdio.h>
// OpenGL and GLM Headers
#include <glut/glut.h>
#include <opengl/gl.h>
//#include "glm.hpp"

using namespace std;

class var3d
{
public:
    double var[3];
};


class face
{
public:
    int v[4];
    int uv[4];
    int nv[4];
};

class f
{
public:
    int f_list_type;
    // Type 1:  a_v_index b_v_index c_v_index
    // Type 2: a_v_index/a_uv_index ... ...
    // Type 3: a_v_index/a_vn_index/a_uv_index ... ...
    // Type 4: a_v_index//a_vn_index ... ...
    int f_list_face_num;
    // Mainly 3 or 4
    vector<face> f_list_face;
};

class Obj
{
public:
    vector<var3d> v_list;
    vector<var3d> vn_list;
    vector<f> f_list;
    
    Obj() {}
    bool loadobj(string filepath);
};

#endif /* OBJ_H */
