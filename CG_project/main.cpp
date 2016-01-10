//project
#include "main1.h"
#include <iostream>

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
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
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
	glGenTextures(100, textureVideo);

	char videoLoc[14] = "video/000.bmp";
    for (int i = 0; i < 110; i++) {
        videoLoc[6] = i / 100 + '0';
		videoLoc[7] = i % 100 / 10 + '0';
		videoLoc[8] = i % 100 % 10 + '0';
        cout << videoLoc << endl;
		loadTexture(i, videoLoc, true);
	}
    loadTexture(101, "data.bmp", true); //防止泄漏
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
    
    glBindTexture(GL_TEXTURE_2D, textureVideo[99]);
    //glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    
    // »æÖÆºóÃæ
//    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
//    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
//    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
//    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
//    
//    // ÉÏÃæ
//    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
//    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
//    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
//    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
//    
//    //µ×Ãæ
//    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
//    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
//    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
//    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
//    
//    // ÓÒÃæ
//    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
//    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
//    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
//    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
//    
//    // ×óÃæ
//    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
//    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
//    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
//    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    
    glEnd();
    
	glPopMatrix();
}

void renderVideo() {
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(0, 0, 0.0);
	glScalef(1.5, 0.6, 0.2);
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

vertex normal(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3])
{
	vertex n;
	GLfloat vc1[3], vc2[3];
	GLfloat a, b, c;
	GLdouble r;
	vc1[0] = v2[0] - v1[0]; vc1[1] = v2[1] - v1[1]; vc1[2] = v2[2] - v1[2];
	vc2[0] = v3[0] - v2[0]; vc2[1] = v3[1] - v2[1]; vc2[2] = v3[2] - v2[2];
	a = vc1[1] * vc2[2] - vc2[1] * vc1[2];
	b = vc2[0] * vc1[2] - vc1[0] * vc2[2];
	c = vc1[0] * vc2[1] - vc2[0] * vc1[1];
	r = sqrt(a * a + b* b + c * c);
	n.pos[0] = -a / r;
	n.pos[1] = -b / r;
	n.pos[2] = -c / r;
	return n;
}

void drawObj(int cur)
{
	while (!v.empty())  v.pop_back();
	while (!vt.empty()) vt.pop_back();

	FILE *fin = fopen(objFileName[cur], "r");

	char str[MAXLENGTH], c[MAXLENGTH];
	bool flag = false;

	while (fgets(str, MAXLENGTH, fin) != NULL)
	{
		sscanf(str, "%s", c);
		if (c[0] == '#') continue;
		if (c[0] == 'v')
		{
			if (c[1] == 't')
			{
				vertexTexture vt0;
				sscanf(str, "%s %f %f", c, &vt0.pos[0], &vt0.pos[1]);
				vt.push_back(vt0);
			}
			else //v
			{
				vertex v0;
				sscanf(str, "%s %f %f %f", c, &v0.pos[0], &v0.pos[1], &v0.pos[2]);
				v.push_back(v0);
			}
		}
		else
		{
			flag = true;
			break;
		}
	}
	while (flag)
	{
		sscanf(str, "%s", c);
		if (c[0] == 'p')
		{
			int v0;
			sscanf(str, "%s %d", c, &v0);
			glBegin(GL_POINTS);
			glVertex3fv(v[v0 - 1].pos);
			glEnd();
		}
		else if (c[0] == 'l')
		{
			int v0, v1;
			sscanf(str, "%s %d %d", c, &v0, &v1);
			glBegin(GL_LINES);
			glVertex3fv(v[v0 - 1].pos);
			glVertex3fv(v[v1 - 1].pos);
			glEnd();
		}
		else if (c[0] == 'f')
		{
			int v0, v1, v2, i;
			sscanf(str, "%s %d %d %d", c, &v0, &v1, &v2);
            cout << str << endl;
			vertex n = normal(v[v0 - 1].pos, v[v1 - 1].pos, v[v2 - 1].pos);
			glBegin(GL_POLYGON);
			glNormal3fv(n.pos);

			i = 0;
			v0 = 0;
			while (str[i] != '\0')
			{
				if (str[i] >= '0'&&str[i] <= '9')
					v0 = v0 * 10 + str[i] - '0';
				else if (v0)
				{
					if (!vt.empty()) glTexCoord2f(vt[v0 - 1].pos[0], vt[v0 - 1].pos[1]);
					glVertex3fv(v[v0 - 1].pos);
					v0 = 0;
				}
				i++;
			}
			if (v0)
			{
				if (!vt.empty()) glTexCoord2f(vt[v0 - 1].pos[0], vt[v0 - 1].pos[1]);
				glVertex3fv(v[v0 - 1].pos); \
			}

			glEnd();
		}

		if (fgets(str, MAXLENGTH, fin) == NULL) break;
	}
	fclose(fin);
}

GLint generateList()
{
	GLint lid = glGenLists(objNum);
	for (int i = 0; i<objNum; i++)
	{
		glNewList(lid + i, GL_COMPILE);
		glPushMatrix();
		drawObj(i);
		glPopMatrix();
		glEndList();
	}
	return lid;
}

void makeMandelbrotImage(int iterationCnt) {
	for (int i = 0; i < mandelbrotSize; i++)
	for (int j = 0; j < mandelbrotSize; j++) {
		float C_re = centerRe - radius + (2.0*radius*i) / mandelbrotSize;
		float C_im = centerIm - radius + (2.0*radius*j) / mandelbrotSize;
		int colorValue = getMandelbrot(C_re, C_im, iterationCnt);
		mandelbrotImage[j][i][0] = colorValue<1 ? 22 : 256 - 4 * colorValue;
		mandelbrotImage[j][i][1] = colorValue<1 ? 24 : 256 - 4 * colorValue;
		mandelbrotImage[j][i][2] = colorValue<1 ? 35 : 256 - 4 * colorValue;
	}
}

void initMandelbrotVideo() {
	glGenTextures(67, textureMandelbrot);
	for (int i = 0; i < 67; i++) {
		glBindTexture(GL_TEXTURE_2D, textureMandelbrot[i]);
		makeMandelbrotImage(i);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mandelbrotSize, mandelbrotSize, 0, GL_RGB, GL_UNSIGNED_BYTE, mandelbrotImage);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
}

void renderMandelbrotVideo() {
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(-2.0f, 0, 0.0f);
	glScalef(1.0, 1.0, 0.2);
	glBindTexture(GL_TEXTURE_2D, textureMandelbrot[displayFPS]);
	renderVideoFrame(1.0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
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

protectedBox::protectedBox() {
	xMin = xMax = yMin = yMax = zMin = zMax = 0.0f;
}

void protectedBox::putCoordinate(float xMinTmp, float xMaxTmp, float yMinTmp, float yMaxTmp, float zMinTmp, float zMaxTmp) {
	xMin = xMinTmp, xMax = xMaxTmp;
	yMin = yMinTmp, yMax = yMaxTmp;
	zMin = zMinTmp, zMax = zMaxTmp;
}

bool protectedBox::isLegal(float x, float y, float z) {
	return !(xMin < x && x < xMax && yMin < y && y < yMax && zMin < z && z < zMax);
}

void key(unsigned char k, int x, int y) {
	static char titleBuffer[256];
	static char snapScreenFileName[256];
	static int snapScreenCnt;
	bool titleStateChanged = false;
	float eyeDeltaX = 0.5 * (eye[0] - center[0]) / 1.5;
	float eyeDeltaZ = 0.5 * (eye[2] - center[2]) / 1.5;
	switch (k) {
	case '0':{ operation = 0; break; }
	case '1':{ operation = 1; break; }
	case '2':{ operation = 2; break; }
	case '3':{ operation = 3; break; }
	case 27:
	case 'q': {exit(0); break; }
	//case '=': {scale = scale + 0.2; break; }
	//case '-': {scale = scale - 0.2; break; }
	case 'o': {cc_color=!cc_color; break; }
	case ' ':{ cc_act = !cc_act; break; }
	case 'd': {//todo, hint: eye[] and center[] are the keys to solve the problems
				  //  angle += 5;

				  if (operation == 0){
					  cc_lightPos[0] += cc_lightPosVar;
				  }
				  else if (operation == 1){
					  if (cc_lightColor[0] + cc_lightColorVar <= 1.0)
						  cc_lightColor[0] += cc_lightColorVar;
				  }
				  else if (operation == 2){
					  cc_eye[0] += cc_eyeVar;
				  }
				  if (operation == 3)
				  {
                     // angle = angle - 5;
                      if (angle > 0 ){
                      dir[0] = center[0] - eye[0];
					  dir[2] = center[2] - eye[2];
					//  printf("dir0=%f,dir2=%f\n", dir[0], dir[2]);
					  dir[0] = dir[0] * cos(c * 5) - dir[2] * sin(c * 5);
					  dir[2] = dir[0] * sin(c * 5) + dir[2] * cos(c * 5);
					  center[0] = eye[0] + dir[0];
					  center[2] = eye[2] + dir[2];
                      }
				  }
				 
				  break;
	}
	case 'a': {//todo
				  // angle -= 5;
				  if (operation == 0){
					  cc_lightPos[0] -= cc_lightPosVar;
				  }
				  else if (operation == 1){
					  if (cc_lightColor[0] - cc_lightColorVar >= 0.0)
						  cc_lightColor[0] -= cc_lightColorVar;
				  }
				  else if (operation == 2){
					  cc_eye[0] -= cc_eyeVar;
				  }
				 else  if (operation == 3)
				  {
                   //   angle = angle + 5;
                      if (angle < 180 ){
					dir[0] = center[0] - eye[0];
					  dir[2] = center[2] - eye[2];
					//  printf("dir0=%f,dir2=%f\n", dir[0], dir[2]);
					  dir[0] = dir[0] * cos(c*(-5)) - dir[2] * sin(c*(-5));
					  dir[2] = dir[0] * sin(c*(-5)) + dir[2] * cos(c*(-5));
					  center[0] = eye[0] + dir[0];
					  center[2] = eye[2] + dir[2];
                      }
				  }
				  
				  break;
	}
	case 's': {//todo
				  // collision test
                titleStateChanged = false;
				 /* for (int i = 0; i < objectNow; ++i) {
					  bool isNextStepLegal = true;
					  short samplingPointCnt = 10;
					  for (int k = 1; k <= samplingPointCnt; ++k)
						  isNextStepLegal &= protectedBoxs[i].isLegal(eye[0] + k / samplingPointCnt*eyeDeltaX, eye[1], eye[2] + k / samplingPointCnt*eyeDeltaZ);
					  if (!isNextStepLegal) {
						  printf("Graphics Course Project: [Warning] Collision!");
						  glutSetWindowTitle(titleBuffer);
						  titleStateChanged = true;
						  break;
					  }
				  }*/
				  if (titleStateChanged) break;
				  // view transform
				  if (operation == 0){
					  cc_lightPos[1] -= cc_lightPosVar;
				  }
				  else if (operation == 1){
					  if (cc_lightColor[1] - cc_lightColorVar >= 0.0)
						  cc_lightColor[1] -= cc_lightColorVar;
				  }
				  else if (operation == 2){
					  cc_eye[1] -= cc_eyeVar;
				  }
				  if (operation == 3)
				  {
				//	printf("e0=%f,e2=%f\n", eye[0], eye[2]);
				  dir[0] = eye[0] - center[0];
				  dir[2] = eye[2] - center[2];
				//  printf("d0=%f,d2=%f\n", dir[0], dir[2]);
				  eye[0] += eyeDeltaX;
				  eye[2] += eyeDeltaZ;
				//  printf("e0=%f,e2=%f\n", eye[0], eye[2]);
				  center[0] = eye[0] - dir[0];
				  center[2] = eye[2] - dir[2];
				  }
				  
				  break;
	}
	case 'w': {//todo
				  // collision test
                titleStateChanged = false;
				 /* for (int i = 0; i < objectNow; ++i) {
					  bool isNextStepLegal = true;
					  short samplingPointCnt = 10;
					  for (int k = 1; k <= samplingPointCnt; ++k)
						  isNextStepLegal &= protectedBoxs[i].isLegal(eye[0] - eyeDeltaX, eye[1], eye[2] - eyeDeltaZ);
					  if (!isNextStepLegal) {
						  printf("Graphics Course Project: [Warning] Collision!");
						  glutSetWindowTitle(titleBuffer);
						  titleStateChanged = true;
						  break;
					  }
				  }*/
				  if (titleStateChanged) break;
				  // view transform
				  if (operation == 0){
					  cc_lightPos[1] += cc_lightPosVar;
				  }
				  else if (operation == 1){
					  if (cc_lightColor[1] + cc_lightColorVar <= 1.0)
						  cc_lightColor[1] += cc_lightColorVar;
				  }
				  else if (operation == 2){
					  cc_eye[1] += cc_eyeVar;
				  }
				  if (operation == 3)
				  {
//printf("e0=%f,e2=%f\n", eye[0], eye[2]);
				  dir[0] = eye[0] - center[0];
				  dir[2] = eye[2] - center[2];
//				  printf("d0=%f,d2=%f\n", dir[0], dir[2]);
				  eye[0] -= eyeDeltaX;
				  eye[2] -= eyeDeltaZ;
//				  printf("e0=%f,e2=%f\n", eye[0], eye[2]);
				  center[0] = eye[0] - dir[0];
				  center[2] = eye[2] - dir[2];
				  }
				  
				  break;
	}
	case 'z': {//todo
				  if (operation == 0){
					  cc_lightPos[2] -= cc_lightPosVar;
				  }
				  else if (operation == 1){
					  if (cc_lightColor[2] - cc_lightColorVar >= 0.0)
						  cc_lightColor[2] -= cc_lightColorVar;
				  }
				  else if (operation == 2){
					  cc_eye[2] -= cc_eyeVar;
				  }
				  break;
	}
	case 'c': {//todo
				  if (operation == 0){
					  cc_lightPos[2] += cc_lightPosVar;
				  }
				  else if (operation == 1){
					  if (cc_lightColor[2] + cc_lightColorVar <= 1.0)
						  cc_lightColor[2] += cc_lightColorVar;
				  }
				  else if (operation == 2){
					  cc_eye[2] += cc_eyeVar;
				  }
				  break;
	}
	case 'p':{
				 sprintf(snapScreenFileName, "ScreenShot%03d.bmp", ++snapScreenCnt);
				 snapScreen(screenWidth, screenHeight, snapScreenFileName);
	}
	
	case 'j':{
				 if (operation == 0){
					 lightPos[0] -= lightPosVar;
				 }
				 else if (operation == 1){
					 if (lightColor[0] - lightColorVar >= 0.0)
						 lightColor[0] -= lightColorVar;
				 }
				 else if (operation == 2){
					 eye[0] -= eyeVar;
				 }
				 break;
	}
	case 'l':{
				 if (operation == 0){
					 lightPos[0] += lightPosVar;
				 }
				 else if (operation == 1){
					 if (lightColor[0] + lightColorVar <= 1.0)
						 lightColor[0] += lightColorVar;
				 }
				 else if (operation == 2){
					 eye[0] += eyeVar;
				 }
				 break;
	}
	case 'k':{
				 if (operation == 0){
					 lightPos[1] -= lightPosVar;
				 }
				 else if (operation == 1){
					 if (lightColor[1] - lightColorVar >= 0.0)
						 lightColor[1] -= lightColorVar;
				 }
				 else if (operation == 2){
					 eye[1] -= eyeVar;
				 }
				 break;
	}
	case 'i':{
				 if (operation == 0){
					 lightPos[1] += lightPosVar;
				 }
				 else if (operation == 1){
					 if (lightColor[1] + lightColorVar <= 1.0)
						 lightColor[1] += lightColorVar;
				 }
				 else if (operation == 2){
					 eye[1] += eyeVar;
				 }
				 break;
	}
	case 'n':{
				 if (operation == 0){
					 lightPos[2] -= lightPosVar;
				 }
				 else if (operation == 1){
					 if (lightColor[2] - lightColorVar >= 0.0)
						 lightColor[2] -= lightColorVar;
				 }
				 else if (operation == 2){
					 eye[2] -= eyeVar;
				 }
				 break;
	}
	case 'm':{
				 if (operation == 0){
					 lightPos[2] += lightPosVar;
				 }
				 else if (operation == 1){
					 if (lightColor[2] + lightColorVar <= 1.0)
						 lightColor[2] += lightColorVar;
				 }
				 else if (operation == 2){
					 eye[2] += eyeVar;
				 }
				 break;
	}
	}

	if (!titleStateChanged)
		sprintf(titleBuffer, "Graphics Course Project"),
		glutSetWindowTitle(titleBuffer);
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

void cc_Draw_Element()
{
	
	//the lamp
	glPushMatrix();
	glTranslatef(cc_lightPos[0] + 8, cc_lightPos[1], cc_lightPos[2]);
	//glRotatef(fRotate, 0, 1, 0);
	//glCallList(list + 8);
	//glutSolidTeapot(1);
	glScalef(0.5, 0.5, 0.5);
	glutSolidSphere(1, 50, 50);
	//glutSolidCube(1);
	glPopMatrix();
	//glEnable(GL_MATERIAL);
	if (cc_color)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, cc_golden);

		glMaterialfv(GL_FRONT, GL_DIFFUSE, cc_golden);

		glMaterialfv(GL_FRONT, GL_SPECULAR, cc_golden);

		glMaterialf(GL_FRONT, GL_SHININESS, cc_shininess);

		glMaterialfv(GL_FRONT, GL_EMISSION, cc_none);
	}
	else
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, cc_cyan);

		glMaterialfv(GL_FRONT, GL_DIFFUSE, cc_cyan);

		glMaterialfv(GL_FRONT, GL_SPECULAR, cc_cyan);

		glMaterialf(GL_FRONT, GL_SHININESS, cc_shininess);

		glMaterialfv(GL_FRONT, GL_EMISSION, cc_none);
	}
	glPushMatrix();
	glTranslatef(0,0,3.5);
	glPushMatrix();
	glTranslatef(10, 0, -4);
	glutSolidSphere(1, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15, 0, 0);
	glScalef(cc_Rotate / 180, cc_Rotate / 180, cc_Rotate / 180);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, -1, -4);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, cc_Rotate / 180, 0);
	glutSolidCone(1, 2, 100, 100);
	glPopMatrix();

	float R = 1;
	int n = 30;
	//cylinder
	glPushMatrix();
	glTranslatef(15, -1, -4);
	glRotatef(-90, 1, 0, 0);
	GLUquadricObj *objCylinder = gluNewQuadric();
	gluCylinder(objCylinder, 1, 1, 2, 100, 100);
	glTranslatef(0, 0, 2);
	glBegin(GL_POLYGON);
	for (int i = n - 1; i >= 0; i--)
		glVertex2f(R*cos(2 * PI / n*i), R*sin(2 * PI / n*i));
	glEnd();
	glPopMatrix();

	n = 6;
	glPushMatrix();
	glTranslatef(10, -1, 0);
	glRotatef(-90, 1, 0, 0);
	GLUquadricObj *objCylinder1 = gluNewQuadric();
	gluCylinder(objCylinder, 1, 1, 2, 6, 6);
	glTranslatef(0, 0, 2);
	glRotatef(30, 0, 0, 1);
	glBegin(GL_POLYGON);
	for (int i = n - 1; i >= 0; i--)
		glVertex2f(R*cos(2 * PI / n*i), R*sin(2 * PI / n*i));
	glEnd();
	glPopMatrix();

	n = 4;
	glPushMatrix();
	glTranslatef(20, -1, 0);
	glRotatef(-90, 1, 0, 0);
	glRotatef(cc_Rotate, 0, 0, 1);
	GLUquadricObj *objCylinder2 = gluNewQuadric();
	gluCylinder(objCylinder, 1.6, 1, 2, 4, 4);
	glTranslatef(0, 0, 2);
	//glRotatef(30, 0, 0, 1);
	glBegin(GL_POLYGON);
	for (int i = n - 1; i >= 0; i--)
		glVertex2f(R*cos(2 * PI / n*i), R*sin(2 * PI / n*i));
	glEnd();
	glPopMatrix();
	glPopMatrix();
	if (cc_act)
	{
		cc_Rotate += 2;
		if (cc_Rotate > 360)
			cc_Rotate = 0;
	}
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, cc_orginal_amb);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, cc_orginal_dif);

	glMaterialfv(GL_FRONT, GL_SPECULAR, cc_orginal_spe);

	glMaterialf(GL_FRONT, GL_SHININESS, 0);

	glMaterialfv(GL_FRONT, GL_EMISSION, cc_none);
}

void trimSurfaceNURBS() {
	gluBeginTrim(theNurb);
	gluPwlCurve(theNurb, 5, &edgePt[0][0], 2, GLU_MAP1_TRIM_2);
	gluEndTrim(theNurb);

	gluBeginTrim(theNurb);
	gluNurbsCurve(theNurb, nbkntcu, curveKnots, 2, &curvePt[0][0], 4, GLU_MAP1_TRIM_2);
	gluPwlCurve(theNurb, nbkpt, &pwlPt[0][0], 2, GLU_MAP1_TRIM_2);
	gluEndTrim(theNurb);
}

void initSurfaceNURBS() {
	int u, v;

	static float xMin = 1000.0f, xMax = -1000.0f;
	static float yMin = 1000.0f, yMax = -1000.0f;
	static float zMin = 1000.0f, zMax = -1000.0f;

	for (u = 0; u < 8; u++)
	for (v = 0; v < 8; v++){
		ctlPoints[u][v][0] = 2.0f + 0.2f*(4.0f - u);
		ctlPoints[u][v][2] = 0.04f*(log(v + 0.001) - 4.0f);
		ctlPoints[u][v][1] = ((rand() % 1000) / 1000.0F - 0.5F)*0.1f + 0.2f*(4.0f - v) + 0.2*(u % 2 ? 1 : 0) + 0.2*(u % 4 ? 1 : 0);
		if (!setProtectedBoxNURBS)
			xMin = min(xMin, ctlPoints[u][v][0]),
			xMax = max(xMax, ctlPoints[u][v][0]),
			yMin = min(yMin, ctlPoints[u][v][2]),
			yMax = max(yMax, ctlPoints[u][v][2]),
			zMin = min(zMin, ctlPoints[u][v][1]),
			zMax = max(zMax, ctlPoints[u][v][1]);
	}
	if (!setProtectedBoxNURBS)
		protectedBoxs[objectNow++].putCoordinate(xMin, xMax, yMin, yMax + 0.05, zMin, zMax),
		//printf("(%.2lf, %.2lf, %.2lf) (%.2lf, %.2lf, %.2lf)\n", xMin, yMin, zMin, xMax, yMax, zMax),
		setProtectedBoxNURBS = true;
	theNurb = gluNewNurbsRenderer();
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 10.0);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);//GLU_OUTLINE_POLYGON); 
	gluNurbsProperty(theNurb, GLU_CULLING, GL_FALSE);
	gluNurbsProperty(theNurb, GLU_AUTO_LOAD_MATRIX, GL_TRUE);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	glPointSize(5.0);
	glLineWidth(2.0);
}

void drawFrameOfNURBS() {
	int u, v;

	glDisable(GL_LIGHTING);
	glColor3f(0.f, 1.f, 0.f);

	glBegin(GL_POINTS);
	for (u = 0; u<8; u++) {
		for (v = 0; v<8; v++) {
			glVertex3fv(ctlPoints[u][v]);
		}
	}
	glEnd();

	glColor3f(1.f, 1.f, 1.f);
	for (u = 0; u<8; u++) {
		glBegin(GL_LINE_STRIP);
		for (v = 0; v<8; v++) {
			glVertex3fv(ctlPoints[u][v]);
		}
		glEnd();
		glBegin(GL_LINE_STRIP);
		for (v = 0; v<8; v++) {
			glVertex3fv(ctlPoints[v][u]);
		}
		glEnd();
	}

	glEnable(GL_LIGHTING);
}

void drawSurfaceNURBS(bool trim) {
	GLfloat tknots[4] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat knots[16] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureNURBS[0]);
	gluBeginSurface(theNurb);

	gluNurbsSurface(theNurb, 4, tknots, 4, tknots, 2 * 2, 2, &tcoords[0][0][0], 2, 2, GL_MAP2_TEXTURE_COORD_2);
	gluNurbsSurface(theNurb, 16, knots, 16, knots, 8 * 3, 3, &ctlPoints[0][0][0], 8, 8, GL_MAP2_VERTEX_3);

	if (trim)
		trimSurfaceNURBS();

	gluEndSurface(theNurb);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

int getMandelbrot(float C_re, float C_im, int maxCount) {
	int count = 0;
	static int previousMaxCount;
	static float Z_re;
	static float Z_im;
	if (maxCount != previousMaxCount + 1)
		Z_re = Z_im = 0,
		previousMaxCount = 0;
	else
		count = previousMaxCount,
		previousMaxCount = maxCount;
	float magnitude = 0;
	float max_magnitude = 1000000;

	// calucalte Z=Z^2+C for given C
	while (magnitude < max_magnitude && count++ < maxCount) {
		float N_re = Z_re * Z_re - Z_im * Z_im + C_re;
		float N_im = Z_re * Z_im * 2 + C_im;
		Z_re = N_re;
		Z_im = N_im;
		magnitude = Z_re * Z_re + Z_im * Z_im;
	}

	if (count >= maxCount)
		return 0;
	else
		return count;
}

// create a single component texture map
void genTextureNURBS(int maxs, int maxt) {
	for (int t = 0; t < maxt; t++) {
		for (int s = 0; s < maxs; s++) {
			float C_re = centerRe - radius + (1.4*radius*s) / maxs;
			float C_im = centerIm - radius + (1.4*radius*t) / maxt;
			int colorValue = getMandelbrot(C_re, C_im, 256);
			checkImage[s][t][0] = colorValue<1 ? 115 : 31 + colorValue / 2.0;
			checkImage[s][t][1] = colorValue<1 ? 151 : 92 + colorValue / 2.0;
			checkImage[s][t][2] = colorValue<1 ? 171 : 123 + colorValue / 2.0;
		}
	}
}

void initTextureNURBS() {
	glGenTextures(1, textureNURBS);
	genTextureNURBS(TEXW, TEXH);
	glBindTexture(GL_TEXTURE_2D, textureNURBS[0]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXW, TEXH, 0, GL_RGB, GL_UNSIGNED_BYTE, checkImage);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void initGlobalScene() {
	glClearColor(0.0, 0.0, 0.3, 1.0);
    initSurfaceNURBS();
	// initialize OpenGL lighting
	/*
	GLfloat matAmb[4] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat matDiff[4] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat matSpec[4] = { 0,0,0, 0 };

	GLfloat lightPos[] = { 10.0, 10.0, 10.0, 0.0 };
	GLfloat lightAmb[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightDiff[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightSpec[4] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);

	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpec);

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL_EXT, GL_SEPARATE_SPECULAR_COLOR_EXT);
	GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);

	glEnable(GL_LIGHT1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		dir[0], dir[1], dir[2]);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor);
	glEnable(GL_LIGHT0);
	*/
//	initTextureNURBS();
	initTextureVideo();
//	initMandelbrotVideo();

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);//GL_MODULATE);

	// collision test: given vertex (xMin, yMin, zMin) to (xMax, yMax, zMax)
	protectedBoxs[objectNow++].putCoordinate(-0.75, 0.75, -0.3, 0.3, -0.1, 0.1);
	protectedBoxs[objectNow++].putCoordinate(-0.5 - 2, -0.5 - 2, -0.5, 0.5, -0.1, 0.1);
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

GLuint lamp, sofa, coffeeTable, settee, sideTable1, sideTable2;
GLuint stage, kotori;

GLuint qh_drawOBJ(char * filename){
    GLMmodel *glm_model;
    GLuint list;
    glm_model = glmReadOBJ(filename);
    glmUnitize(glm_model);
    glmScale(glm_model, 5);
    glmFacetNormals(glm_model);
    glmVertexNormals(glm_model, 90);
    list = glmList(glm_model, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
    glmDelete(glm_model);
    return list;
}

void loadOBJ(){
    lamp = qh_drawOBJ("model/plant.obj");
    settee = qh_drawOBJ("model/WoodTable.obj");
    stage = qh_drawOBJ("model/sbk_stage.obj");
    kotori = qh_drawOBJ("model/Minami_Kotori.obj");
    
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
	//god lu's camera
	if (operation==3)
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0); //从视点看远点,y轴方向(0,1,0)是上方向

	//cc's camera
	else gluLookAt(cc_eye[0], cc_eye[1], cc_eye[2],
		cc_center[0], cc_center[1], cc_center[2],
		cc_up[0], cc_up[1], cc_up[2]);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, cc_lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, cc_lightColor);
	glEnable(GL_LIGHT0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glScalef(scale, scale, scale);
	glTranslatef(5, 0, -4);
	renderVideo();
	renderMandelbrotVideo();

//	initSurfaceNURBS();
	//drawFrameOfNURBS();
//	drawSurfaceNURBS(true);
	glPopMatrix();
	//floor
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureObjects[0]);
	glPushMatrix();
	glTranslatef(0, -1, 0);
	glScalef(15, 8, 8);
//	glCallList(list + 0); //地板
	glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, textureObjects[2]);
    glPushMatrix();
    glTranslatef(15, 4, 0);
    glScalef(0.2, 5, 8);
//    glCallList(list + 2);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-10, 4, -8);
    glScalef(5, 5, 0.2);
//    glCallList(list + 2); //墙
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-5, 4, -8);
    glScalef(5, 5, 0.2);
//    glCallList(list + 2);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(5, 4, -8);
    glScalef(5, 5, 0.2);
//    glCallList(list + 2);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(10, 4, -8);
    glScalef(5, 5, 0.2);
//    glCallList(list + 2);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-14.5, 4, 0);
    glScalef(0.2, 5, 8);
//    glCallList(list + 2);
    glPopMatrix();
    
	glDisable(GL_TEXTURE_2D);
	glTranslatef(-8, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureObjects[1]);
	glPushMatrix();
	glTranslatef(4, 0, 2);
//	glCallList(list + 1);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureObjects[2]);
	glPushMatrix();
	glTranslatef(2, 0, 4);
//	glCallList(list + 2);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureObjects[3]);
	glPushMatrix();
	glTranslatef(-2, 0, 4);
//	glCallList(list + 3);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureObjects[4]);
	glPushMatrix();
	glTranslatef(-4, 0, 2);
//	glCallList(list + 4);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureObjects[5]);
	glPushMatrix();
	glTranslatef(-4, 0, -2);
	glRotatef(fRotate, 0, 1, 0);
//	glCallList(list + 5);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureObjects[6]);
	glPushMatrix();
	glTranslatef(-2, 0, -4);
	glRotatef(fRotate, 0, 1, 0);
//	glCallList(list + 6);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureObjects[7]);
	glPushMatrix();
	glTranslatef(2, 0, -4);
	glRotatef(fRotate, 0, 1, 0);
//	glCallList(list + 7);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureObjects[8]);
	glPushMatrix();
	glTranslatef(4, 0, -2);
	glRotatef(fRotate, 0, 1, 0);
//	glCallList(list + 8);
	//glutSolidTeapot(1);
	//glutSolidSphere(1, 100, 100);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
  
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureObjects[7]);
    glPushMatrix();
   // glColor3f(0.5, 0.5,0.5);
    glScalef(0.5, 0.5, 0.5);
    glTranslatef(12, 6, -4);
   // glRotatef(fRotate, 0, 1, 0);
//    glCallList(lamp);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureObjects[4]);
    glPushMatrix();
    // glColor3f(0.5, 0.5,0.5);
    glScalef(0.3, 0.3, 0.3);
    glTranslatef(23, 0, 0);
    // glRotatef(fRotate, 0, 1, 0);
//    glCallList(settee);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureObjects[8]);
    glPushMatrix();
    glTranslatef(0, 10, 0);
    glScalef(5, 5, 5);
    glCallList(stage);
    glPopMatrix();
     glDisable(GL_TEXTURE_2D);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureObjects[10]);
    glPushMatrix();
    glScalef(0.3, 0.3, 0.3);
    glCallList(kotori);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
//	cc_Draw_Element();
		fRotate += 5;
	if (fRotate == 365) fRotate = 0;

	//getFPS();
	glutSwapBuffers();
}

void getVideoFrame(int value){
	displayFPS++;
	displayFPS = displayFPS % 110;
	display();
	// Present frame every 136 ms
	glutTimerFunc(136, getVideoFrame, 0);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow("Graphics Course Project");
	loadObjectTextures();
	initGlobalScene();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(key);
	glutMotionFunc(onMouseMove);
	glutTimerFunc(136, getVideoFrame, 0);
	list = generateList();
    loadOBJ();
	glutMainLoop();

	return 0;
}
