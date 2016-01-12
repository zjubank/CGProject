//project
#include "main.h"

void readBMPfileHeader(BMPfileHeader &t){
    myfin.read((char*)&t,sizeof(struct BMPfileHeader));
}

void readBMPinfoHeader(BMPinfoHeader &t){
    myfin.read((char*)&t,sizeof(struct BMPinfoHeader));
    if (t.biHeight<0){
        cerr<<"height < 0 !!"<<endl;
    }
}

void readBMPdata(BMP &p){
    p.data=new unsigned char[abs(p.infoHeader.biHeight*((p.infoHeader.biWidth+31)/32*4)*p.infoHeader.biBitCount)];
    myfin.read((char*)p.data,sizeof(unsigned char)*abs(p.infoHeader.biHeight*((p.infoHeader.biWidth+31)/32*4)*p.infoHeader.biBitCount));
}

void readBMPpaletee(BMP &p){
    if (p.infoHeader.biBitCount<24){
        cout<<"Read a palette."<<endl;
        int plsize=(p.infoHeader.biClrUsed==0)?(1<<p.infoHeader.biBitCount):p.infoHeader.biClrUsed;
        cout << plsize << endl;
        p.palette=new BMPpalette[plsize];
        myfin.read((char*)p.palette,sizeof(BMPpalette)*plsize);
    }
}

void readBMP(char fileName[],BMP &p){
    char address[200];
    strcat(address, fileName);
    
    myfin.open(address,ios::binary|ios::in);
    if (!myfin) cout<<"Read Err"<<endl;
    readBMPfileHeader(p.fileHeader);
    readBMPinfoHeader(p.infoHeader);
    readBMPpaletee(p);
    readBMPdata(p);
    myfin.close();
}

void readMTL(char * fileName) {
    ifstream fin(fileName);
    
    string s;
    while (getline(fin,s)){
        string stemp=s.substr(0,6);
        /*if (stemp=="newmtl"){
         string sNum=s.substr()
         textureNum==
         }*/
        
        if (stemp=="map_Kd"){
            string st=s.substr(7);
            st[st.length() - 1] = '\0';
            strcpy(texFileName[textureObjectCnt],st.c_str());
            
            cout<< texFileName[textureObjectCnt] <<endl;
            textureObjectCnt++;
            
        }
    }
    
}

bool snapScreen(int width, int height, const char *file) {
    byte *image;
    FILE *fp;
    BITMAPFILEHEADER FileHeader;
    BITMAPINFOHEADER InfoHeader;
    
    FileHeader.bfType = BITMAP_ID; // BMP identity
    FileHeader.bfOffBits = 14 + sizeof(BITMAPINFOHEADER); // data location
    FileHeader.bfReserved1 = FileHeader.bfReserved2 = 0;
    FileHeader.bfSize = height*width * 24 + FileHeader.bfOffBits; // file size
    InfoHeader.biXPelsPerMeter = InfoHeader.biYPelsPerMeter = 0; // resolution
    InfoHeader.biClrUsed = 0;
    InfoHeader.biClrImportant = 0;
    InfoHeader.biPlanes = 1;
    InfoHeader.biCompression = 0;
    InfoHeader.biBitCount = 24;
    InfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    InfoHeader.biHeight = height;
    InfoHeader.biWidth = width;
    InfoHeader.biSizeImage = height*width * 3;
    
    image = (byte *)malloc(sizeof(byte)*InfoHeader.biSizeImage);
    if (image == NULL) {
        free(image);
        printf("Exception: No enough space!\n");
        return false;
    }
    
    // get output image data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, image);
    
    fp = fopen(file, "wb");
    if (fp == NULL) {
        printf("Exception: Fail to open file!\n");
        return false;
    }
    fwrite(&FileHeader.bfType, 2, 1, fp);
    fwrite(&FileHeader.bfSize, 4, 1, fp);
    fwrite(&FileHeader.bfReserved1, 2, 1, fp);
    fwrite(&FileHeader.bfReserved2, 2, 1, fp);
    fwrite(&FileHeader.bfOffBits, 4, 1, fp);
    fwrite(&InfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
    fwrite(image, InfoHeader.biSizeImage, 1, fp);
    free(image);
    fclose(fp);
    return true;
}

unsigned char *loadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
    FILE *filePtr;  // 文件指针
    BITMAPFILEHEADER bitmapFileHeader;  // bitmap文件头
    unsigned char   *bitmapImage;       // bitmap图像数据
    int imageIdx = 0;       // 图像位置索引
    unsigned char   tempRGB;    // 交换变量
    
    filePtr = fopen(filename, "rb");
    if (filePtr == NULL) {
        fprintf(stderr, "Error in loadBitmapFile: the file doesn't exist\n");
        return NULL;
    }
    fread(&bitmapFileHeader, 14, 1, filePtr);
    if (bitmapFileHeader.bfType != BITMAP_ID) {
        fprintf(stderr, "Error in loadBitmapFile: the file is not a bitmap file\n");
        return NULL;
    }
    
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
    bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage + 2];
    if (!bitmapImage) {
        fprintf(stderr, "Error in loadBitmapFile: memory error\n");
        return NULL;
    }
    
    fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
    if (bitmapImage == NULL) {
        fprintf(stderr, "Error in loadBitmapFile: memory error\n");
        return NULL;
    }
    
    for (imageIdx = 0;
         imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }
    fclose(filePtr);
    return bitmapImage;
}


void loadTexture(int i, char* filename, bool type) {
    BITMAPINFOHEADER bitmapInfoHeader;
    unsigned char*   bitmapData;
    bitmapData = loadBitmapFile(filename, &bitmapInfoHeader);
    // bind the texture
    glBindTexture(GL_TEXTURE_2D, type ? textureVideo[i] : textureObjects[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);
}


void initTextureVideo() {
    glGenTextures(30, textureVideo);
    
    char videoLoc[14] = "video/000.bmp";
    for (int i = 0; i <= 29; i++) {
        videoLoc[7] = i / 10 + '0';
        videoLoc[8] = i % 10 + '0';
        cout << videoLoc << endl;
        loadTexture(i, videoLoc, true);
    }
    loadTexture(30, "black.bmp", true);
}

void renderVideoFrame(GLfloat x) {
    glPushMatrix();
    glScalef(x / 2, x / 2, x / 2);
    
    //  glBindTexture(GL_TEXTURE_2D, textureVideo[displayFPS]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    
    
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, textureVideo[30]);
    //glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    
    // »æÖÆºóÃæ
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    
    // ÉÏÃæ
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    
    //µ×Ãæ
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    
    // ÓÒÃæ
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    
    // ×óÃæ
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    
    glEnd();
    
    glPopMatrix();
}

void renderVideo() {
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(0, 0, 0.0);
    glScalef(1.6, 0.9, 0.0);
    // update the video's output frame by frame
    glBindTexture(GL_TEXTURE_2D, textureVideo[displayFPS]);
    renderVideoFrame(1.0);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void loadObjectTextures()
{
    glGenTextures(textureObjectCnt, textureObjects);        // µÚÒ»²ÎÊýÊÇÐèÒªÉú³É±êÊ¾·ûµÄ¸öÊý, µÚ¶þ²ÎÊýÊÇ·µ»Ø±êÊ¾·ûµÄÊý×é
    
    for (int i = 0; i<textureObjectCnt; i++)
        loadTexture(i, texFileName[i], false);
}

int Pointer_Count = 0;
int Time = 0;

GLvoid draw_pointer(){
    
    Pointer_Count++;
    if (Pointer_Count == 10){
        Pointer_Count = 0;
        Time++;
        if (Time == 12 * 3600)
            Time = 0;
    }
    
    double hour, min, sec;
    hour = Time / 3600;
    sec = Time % 60;
    min = Time / 60 - hour * 60;
    
    //glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0.1);
    glVertex3f(0.5*sin(((hour + min / 60 + sec / 3600) * 6 + 5) / 180 * M_PI), 0.5*cos(((hour + min / 60 + sec / 3600) * 6 + 5) / 180 * M_PI), 0.03);
    glVertex3f(0.6*sin((hour + min / 60 + sec / 3600) * 6 / 180 * M_PI), 0.6*cos((hour + min / 60 + sec / 3600) * 6 / 180 * M_PI), 0.03);
    glVertex3f(0.5*sin(((hour + min / 60 + sec / 3600) * 6 - 5) / 180 * M_PI), 0.5*cos(((hour + min / 60 + sec / 3600) * 6 - 5) / 180 * M_PI), 0.03);
    glEnd();
    
    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0.01);
    glVertex3f(0.6*sin(((min + sec / 60) * 6 + 2) / 180 * M_PI), 0.6*cos(((min + sec / 60) * 6 + 2) / 180 * M_PI), 0.02);
    glVertex3f(0.7*sin((min + sec / 60) * 6 / 180 * M_PI), 0.7*cos((min + sec / 60) * 6 / 180 * M_PI), 0.02);
    glVertex3f(0.6*sin(((min + sec / 60) * 6 - 2) / 180 * M_PI), 0.6*cos(((min + sec / 60) * 6 - 2) / 180 * M_PI), 0.02);
    glEnd();
    
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0.01);
    glVertex3f(0.8*sin(sec*6/180*M_PI), 0.8*cos(sec*6/180*M_PI), 0.01);
    glEnd();
    glPopMatrix();
    //glutSwapBuffers();
    
}

void Draw_cube1(GLfloat x, int Direc){
    
    //glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_BLEND);
    glPushMatrix();
    glScalef(x / 2, x / 2, x / 2);
    int d = Direc;
    glBegin(GL_QUADS);  //∆Ù”√Àƒ±ﬂ–Œ¥¯ªÊ÷∆ƒ£ ΩªÊ÷∆
    if (d / 32 == 1){
        // ªÊ÷∆«∞√Ê£¨’‚¿Ôø™ º»∑∂®Œ∆¿Ì◊¯±Í£¨»ª∫Û «»∑∂®µ„µƒŒª÷√
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
        d -= 32;
    }
    
    if (d / 16 == 1){
        // ªÊ÷∆∫Û√Ê
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
        d -= 16;
    }
    
    if (d / 8 == 1){
        // …œ√Ê
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
        d -= 8;
    }
    
    if (d / 4 == 1){
        //µ◊√Ê
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
        d -= 4;
    }
    
    if (d / 2 == 1){
        // ”“√Ê
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
        d -= 2;
    }
    
    if (d == 1){
        // ◊Û√Ê
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
        d -= 1;
    }
    glEnd();
    
    glPopMatrix(); //µØ≥ˆ±‰ªªæÿ’Û
    
}

GLvoid draw_circle(const GLfloat radius, const GLuint num_vertex)
{
    GLfloat vertex[4];
    GLfloat texcoord[2];
    
    const GLfloat delta_angle = 2.0*M_PI / num_vertex;
    glPushMatrix();
    glTranslatef(0, 0, 3);
    glRotatef(90, 1, 0, 0);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBegin(GL_TRIANGLE_FAN);
    
    
    
    //draw the vertex at the center of the circle
    texcoord[0] = 0.5;
    texcoord[1] = 0.5;
    glTexCoord2fv(texcoord);
    vertex[0] = vertex[1] = vertex[2] = 0.0;
    vertex[3] = 1.0;
    glVertex4fv(vertex);
    
    //draw the vertex on the contour of the circle
    for (int i = 0; i < num_vertex; i++)
    {
        texcoord[0] = (cos(delta_angle*i) + 1.0)*0.5;
        texcoord[1] = (sin(delta_angle*i) + 1.0)*0.5;
        glTexCoord2fv(texcoord);
        
        vertex[0] = cos(delta_angle*i) * radius;
        vertex[1] = sin(delta_angle*i) * radius;
        vertex[2] = 0.0;
        vertex[3] = 1.0;
        glVertex4fv(vertex);
    }
    
    texcoord[0] = (1.0 + 1.0)*0.5;
    texcoord[1] = (0.0 + 1.0)*0.5;
    glTexCoord2fv(texcoord);
    
    vertex[0] = 1.0 * radius;
    vertex[1] = 0.0 * radius;
    vertex[2] = 0.0;
    vertex[3] = 1.0;
    glVertex4fv(vertex);
    
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
}







void Draw_Door(float w, float h, float d){
    glPushMatrix();
    glTranslatef(0, 0, 3);
    glRotatef(90, 1, 0, 0);
    
    glDisable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    
    glScalef(w, h, d);
    Draw_cube1(1.0, 63);
    
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
}

void Draw_Footstep(float w, float h, float d){
    glPushMatrix();
    glTranslatef(0, 0, 3);
    glRotatef(90, 1, 0, 0);
    
    glDisable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    
    glScalef(w, h, d);
    Draw_cube1(1.0, 40);
    
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Draw_Clock(int h, int m, int s){
    glLineWidth(3.0);
    glColor3f(1.0, 0.0, 1.0);
    draw_circle(1.0, 32);
    
    draw_pointer();
    //glFlush();
    
    //glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y)  {
    // record initial coordinate when the mouse clicked fistly
    if (state == GLUT_DOWN)
        oldmx = x,
        oldmy = y;
}

void onMouseMove(int x, int y) {
    du += x - oldmx; //鼠标在窗口x轴方向上的增量加到视点绕y轴的角度上，这样就左右转了
    eye[0] = center[0] + r*cos(c*du);
    eye[2] = center[2] + r*sin(c*du);
    h += 0.03f*(y - oldmy); //鼠标在窗口y轴方向上的改变加到视点的y坐标上，就上下转了
	   if(h>1.0f) h=1.0f; //视点y坐标作一些限制，不会使视点太奇怪
       else if(h<-1.0f) h=-1.0f;
    eye[1] = h;
    oldmx = x, oldmy = y; //把此时的鼠标坐标作为旧值，为下一次计算增量做准备
}

void key(unsigned char k, int x, int y) {
    static char titleBuffer[256];
    static char snapScreenFileName[256];
    static int snapScreenCnt;
    bool titleStateChanged = false;
    float eyeDeltaX = 0.5 * (eye[0] - center[0]) / 1.5;
    float eyeDeltaZ = 0.5 * (eye[2] - center[2]) / 1.5;
    switch (k) {
        case ' ':{ ifmainlight = !ifmainlight; break; } // Main light Switch
        case '0':{ operation = 0; break; } // Main light Pos
        case '1':{ operation = 1; break; } // Main light Color
        case '2':{ operation = 2; break; } // Bird(kotori) view
        case '3':{ operation = 3; break; } // First persion view
        case '5':{ ifspotlight = !ifspotlight; break; } // Spotlight Switch
        case '6':{ ifflashlight = !ifflashlight; break; } // Flashlight Switch
        case 27:
        case 'q': {exit(0); break; }
        case 'd': {
            if (operation == 0){
                MainLightPos[0] += MainLightPosVar;
            }
            else if (operation == 1){
                if (MainLightColor[0] + MainLightColorVar <= 1.0)
                    MainLightColor[0] += MainLightColorVar;
            }
            else if (operation == 2){
                FP_eye[0] += FP_eyeVar;
            }
            else if (operation == 3)
            {
                if (angle > 0 ){
                    dir[0] = center[0] - eye[0];
                    dir[2] = center[2] - eye[2];
                    dir[0] = dir[0] * cos(c * 5) - dir[2] * sin(c * 5);
                    dir[2] = dir[0] * sin(c * 5) + dir[2] * cos(c * 5);
                    center[0] = eye[0] + dir[0];
                    center[2] = eye[2] + dir[2];
                }
            }
            
            break;
        }
        case 'a': {
            if (operation == 0){
                MainLightPos[0] -= MainLightPosVar;
            }
            else if (operation == 1){
                if (MainLightColor[0] - MainLightColorVar >= 0.0)
                    MainLightColor[0] -= MainLightColorVar;
            }
            else if (operation == 2){
                FP_eye[0] -= FP_eyeVar;
            }
            else  if (operation == 3)
            {
                if (angle < 180 ){
                    dir[0] = center[0] - eye[0];
                    dir[2] = center[2] - eye[2];
                    dir[0] = dir[0] * cos(c*(-5)) - dir[2] * sin(c*(-5));
                    dir[2] = dir[0] * sin(c*(-5)) + dir[2] * cos(c*(-5));
                    center[0] = eye[0] + dir[0];
                    center[2] = eye[2] + dir[2];
                }
            }
            
            break;
        }
        case 's': {
            titleStateChanged = false;
            if (titleStateChanged) break;
            if (operation == 0){
                MainLightPos[1] -= MainLightPosVar;
            }
            else if (operation == 1){
                if (MainLightColor[1] - MainLightColorVar >= 0.0)
                    MainLightColor[1] -= MainLightColorVar;
            }
            else if (operation == 2){
                FP_eye[1] -= FP_eyeVar;
            }
            if (operation == 3)
            {
                {
                    dir[0] = eye[0] - center[0];
                    dir[2] = eye[2] - center[2];
                    eye[0] += eyeDeltaX;
                    eye[2] += eyeDeltaZ;
                    center[0] = eye[0] - dir[0];
                    center[2] = eye[2] - dir[2];
                }
            }
            
            break;
        }
        case 'w': {
            titleStateChanged = false;
            if (titleStateChanged) break;
            if (operation == 0){
                MainLightPos[1] += MainLightPosVar;
            }
            else if (operation == 1){
                if (MainLightColor[1] + MainLightColorVar <= 1.0)
                    MainLightColor[1] += MainLightColorVar;
            }
            else if (operation == 2){
                FP_eye[1] += FP_eyeVar;
            }
            if (operation == 3)
            {
                {
                    dir[0] = eye[0] - center[0];
                    dir[2] = eye[2] - center[2];
                    eye[0] -= eyeDeltaX;
                    eye[2] -= eyeDeltaZ;
                    center[0] = eye[0] - dir[0];
                    center[2] = eye[2] - dir[2];
                }
            }
            
            break;
        }
        case 'z': {
            if (operation == 0){
                MainLightPos[2] -= MainLightPosVar;
            }
            else if (operation == 1){
                if (MainLightColor[2] - MainLightColorVar >= 0.0)
                    MainLightColor[2] -= MainLightColorVar;
            }
            else if (operation == 2){
                FP_eye[2] -= FP_eyeVar;
            }
            break;
        }
        case 'c': {
            if (operation == 0){
                MainLightPos[2] += MainLightPosVar;
            }
            else if (operation == 1){
                if (MainLightColor[2] + MainLightColorVar <= 1.0)
                    MainLightColor[2] += MainLightColorVar;
            }
            else if (operation == 2){
                FP_eye[2] += FP_eyeVar;
            }
            break;
        }
        case 'p':{
            sprintf(snapScreenFileName, "ScreenShot%03d.bmp", ++snapScreenCnt);
            snapScreen(screenWidth, screenHeight, snapScreenFileName);
        }
            
    }
    
    //	if (!titleStateChanged)
    //		sprintf(titleBuffer, "CG Project - それは僕たちの奇跡"),
    //		glutSetWindowTitle(titleBuffer);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0f, (float)w / h, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

void idle()
{
    glutPostRedisplay();
}


void initGlobalScene() {
    glClearColor(0.0, 0.0, 0.3, 1.0);
    
    initTextureVideo();
}

void getFPS()
{
    static int frame = 0, time, timebase = 0;
    static char buffer[256] = {0};
    
    char mode[64] = {0};
    
    
    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        sprintf(buffer, "FPS:%4.2f %s",
                frame*1000.0 / (time - timebase), mode);
        timebase = time;
        frame = 0;
    }
    
    glutSetWindowTitle(buffer);
}

GLuint stage, kotori;

GLuint glm_drawOBJ(char * filename){
    GLMmodel *glm_model;
    GLuint list;
    glm_model = glmReadOBJ(filename);
    stage2M=glm_model;
    glmUnitize(glm_model);
    glmScale(glm_model, 20);
    glmFacetNormals(glm_model);
    glmVertexNormals(glm_model, 90);
    list = glmList(glm_model, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
    //glmDelete(glm_model);
    return list;
}

void loadOBJ(){
    stage = glm_drawOBJ("model/stage_big.obj");
    //    kotori = glm_drawOBJ("model/kotori_sbk_test.obj");
    
}

void display() {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    {
        // gradient background
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0.0, screenWidth, 0.0, screenHeight, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glBegin(GL_QUADS);
        glColor3f(0.2, 0.4, 0.8);
        glVertex2f(0.0, 0.0);
        glVertex2f(screenWidth, 0.0);
        glColor3f(0.05, 0.1, 0.2);
        glVertex2f(screenWidth, screenHeight);
        glVertex2f(0, screenHeight);
        glEnd();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1, 0.1f, 1000.0f);
    
    //Bird View
    if (operation==3)
    {
        gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0); //从视点看远点,y轴方向(0,1,0)是上方向
    }
    //First person View
    else
    {
        gluLookAt(FP_eye[0], FP_eye[1], FP_eye[2],
                  FP_center[0], FP_center[1], FP_center[2],
                  FP_up[0], FP_up[1], FP_up[2]);
    }
    
    
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_POSITION, MainLightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, MainLightColor);
    glEnable(GL_LIGHT0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    glPushMatrix();
    glScaled(-1, 1, 1);
    glTranslatef(0, 10, 3);
    glEnable(GL_TEXTURE_2D);
    //textureObjectCnt=6;
    for (int ii = 0; ii < textureObjectCnt; ii++) {
        glBindTexture(GL_TEXTURE_2D, textureObjects[textureObjectCnt-1-ii]);
        GLMmodel *stage2=stage2M;
        //GLMgroup *group=stage2->groups;
        //glPushMatrix();
        //group->
        //glmDraw(stage2->groups,GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
        //        stage2->groups=stage2->groups->next;
        //cout<<texFileName[ii]<<endl;
        glmDrawGroup(stage2, ii,GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
        
        //system("pause");
        //glPopMatrix();
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    
    glPushMatrix();
    glScalef(10, 10, 10);
    glRotatef(180, 0, 1, 0);
    glTranslatef(0, 0.6, -1.2);
    renderVideo();
    glPopMatrix();
    
    glPushMatrix();
    glScalef(1, 1, 1);
    glTranslatef(0, 4, -16.5);
    glRotatef(-90, 1, 0, 0);
    Draw_Clock(8,0,0);
    glPopMatrix();
    
    //    Draw_Door()
    //    Draw_Footstep(<#float w#>, <#float h#>, <#float d#>)
    
    
    fRotate += 5;
    if (fRotate == 365) fRotate = 0;
    
    //getFPS();
    glutSwapBuffers();
}

void getVideoFrame(int value){
    displayFPS++;
    displayFPS = displayFPS % 30;
    display();
    // Present frame every 136 ms
    glutTimerFunc(200, getVideoFrame, 0);
}

void texload(int i, char *filename)
{
    BMP RGBpic;
    readBMP(filename, RGBpic);
    
    glBindTexture(GL_TEXTURE_2D, texture[i]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 RGBpic.infoHeader.biWidth,
                 RGBpic.infoHeader.biHeight,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 RGBpic.data
                 );
}

void gettex()
{
    int i, j;
    for (i = 0; i < n_bmp; i++)
        for (j = 0; j < n_bmp; j++)
        {
            if ((i + j) % 2 == 0)
                tex[i][j][0] = 0;
            else tex[i][j][0] = 0xff;
        }
}

void init()
{
    glGenTextures(3, texture);                                                  //产生纹理标识符
    texload(0, "Monet.bmp");
    texload(1, "Crack.bmp");
    texload(3, "Spot.bmp");
    
    gettex();
    glBindTexture(GL_TEXTURE_2D, texture[2]);                                   //指定当前纹理
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, n_bmp, n_bmp, 0, GL_RGB, GL_UNSIGNED_BYTE, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);           //指定纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
}


int main(int argc, char *argv[]) {
    //    cout << "ok";
    
    stage2M=(GLMmodel*)malloc(sizeof(GLMmodel));
    kotori2M=(GLMmodel*)malloc(sizeof(GLMmodel));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("CG Project - それは僕たちの奇跡");
    readMTL("model/stage_big.mtl");
    //    readMTL("model/kotori_sbk_test.mtl");
    loadObjectTextures();
    initGlobalScene();
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMouseFunc(Mouse);
    glutKeyboardFunc(key);
    glutMotionFunc(onMouseMove);
    glutTimerFunc(200, getVideoFrame, 0);
    //	list = generateList();
    loadOBJ();
    glutMainLoop();
    
    return 0;
}
