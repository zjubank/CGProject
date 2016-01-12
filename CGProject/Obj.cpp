//
//  obj.cpp
//  CGProject
//
//  Created by 尹航 on 16/1/9.
//  Copyright © 2016年 zjubank. All rights reserved.
//

// Class Headers
#include "Obj.h"
#include "Mtl.h"
// C++ Headers
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
// C Headers
#include <stdio.h>
// OpenGL and GLM Headers
#include <glut/glut.h>
#include <opengl/gl.h>
//#include "glm.hpp"

using namespace std;

double str2double( string str )
{
    istringstream iss(str);
    double dou;
    iss >> dou;
    return dou;
}

bool Obj::loadobj( string filename )
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
        
        if( type == "mtllib" )
        {
            
        }
        else if( type == "o" )
        {
            
        }
        else if( type == "v" )
        {
            var3d vtemp;
            
            buffer = buffer.substr(buffer.find(' ')+1, buffer.size());
            vtemp.var[0] = str2double(buffer.substr(0,buffer.find(' ')));

            buffer = buffer.substr(buffer.find(' ')+1, buffer.size());
            vtemp.var[1] = str2double(buffer.substr(0,buffer.find(' ')));

            buffer = buffer.substr(buffer.find(' ')+1, buffer.size());
            vtemp.var[2] = str2double(buffer.substr(0,buffer.find(' ')));

            v_list.push_back(vtemp);
        }
        else if( type == "vn" )
        {
            var3d vntemp;
            
            buffer = buffer.substr(buffer.find(' ')+1, buffer.size());
            vntemp.var[0] = str2double(buffer.substr(0,buffer.find(' ')));
            
            buffer = buffer.substr(buffer.find(' ')+1, buffer.size());
            vntemp.var[1] = str2double(buffer.substr(0,buffer.find(' ')));
            
            buffer = buffer.substr(buffer.find(' ')+1, buffer.size());
            vntemp.var[2] = str2double(buffer.substr(0,buffer.find(' ')));
            
            vn_list.push_back(vntemp);
        }
        else if( type == "usemtl" )
        {
            
        }
        else if( type == "s" )
        {
            
        }
        else if( type == "f" )
        {
            f ftemp;
            
            if( buffer.find('/') != string::npos )
            {
                int pos1 = (int)buffer.find('/');
                string substring = buffer.substr(pos1+1,buffer.find(' '));
                if (substring.find('/') != string::npos )
                {
                    int pos2 = (int)substring.find('/');
                    if( pos2 == 0 )
                    {
                        ftemp.f_list_type = 4;  // Two neighbor '/' between space
                    }
                    else
                    {
                        ftemp.f_list_type = 3; // Two divided '/' between spcaes
                    }
                }
                else
                {
                    ftemp.f_list_type = 2; // Only one '/' between spaces
                }
            }
            else
            {
                ftemp.f_list_type = 1; // No '/'
            }
            cout << "F type:" << ftemp.f_list_type << endl;

            if( ftemp.f_list_type == 1 )
            {
                buffer = buffer.substr(buffer.find(' ')+1, buffer.size());
                
            }
            else if( ftemp.f_list_type == 2 )
            {
                buffer = buffer.substr(buffer.find(' ')+1, buffer.size());
            }
            else if( ftemp.f_list_type == 3 )
            {
                buffer = buffer.substr(buffer.find(' ')+1, buffer.size());
            }
            else if( ftemp.f_list_type == 4 )
            {
                buffer = buffer.substr(buffer.find(' ')+1, buffer.size());
            }

            
            if( buffer.find(' ') != string::npos )
            {
                ftemp.f_list_face_num = 4;
                cout << "This is a Quadrilateral face." << endl;
                buffer = buffer.substr(buffer.find(' ')+1, buffer.size());
                if( ftemp.f_list_type == 1 )
                {
                    
                }
                else if( ftemp.f_list_type == 2 )
                {
                    
                }
                else if( ftemp.f_list_type == 3 )
                {

                }
                else if( ftemp.f_list_type == 4 )
                {

                }
            }
        }
        else
        {
            cerr << "Unknown obj type " << type << endl;
            continue;
        }
    }
    cout << "vnum:" << v_list.size() << endl;
    cout << "vnnum:" << vn_list.size() << endl;
    return true;
}
