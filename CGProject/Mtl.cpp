//
//  mtl.cpp
//  CGProject
//
//  Created by 尹航 on 16/1/9.
//  Copyright © 2016年 zjubank. All rights reserved.
//

// Class Headers
#include "Mtl.h"
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

bool Mtl::loadmtl( string filename )
{
    string buffer;
    ifstream infile;
    
    infile.open(filename);
    if( !infile )
    {
        cerr << "[ERROR] Can not open file." << endl;
        return false;
    }
    
    while(getline(infile, buffer))
    {
        cout << buffer << endl;
        if( buffer.size() == 0 || buffer.at(0) == '#' ) continue;
        
        string type = buffer.substr(0, buffer.find(' '));
        if( type == "newmtl" )
        {
            
        }
        else if( type == "Ns" )
        {
            
        }
        else if( type == "Ka" )
        {
            
        }
        else if( type == "Kd" )
        {
            
        }
        else if( type == "Ks" )
        {
            
        }
        else if( type == "Ke" )
        {
            
        }
        else if( type == "Ni" )
        {
            
        }
        else if( type == "d" )
        {
            
        }
        else if( type == "illum" )
        {
            
        }
        else
        {
            cerr << "Unknown mtl type" << type << endl;
            continue;
        }
    }
    
    return true;
}
