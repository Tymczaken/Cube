#include <stdio.h>
#include <stdint.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include <time.h>
GLubyte PixelBuffer[960*540*3];
#define PI 3.1415926
void PutPixel(int x,int y, uint8_t r, uint8_t g, uint8_t b, GLubyte* Buffer){
	if(x<0 || x>959 || y<0 || y>539){return; }
	int position=x+y*960;
	Buffer[position*3  ]=r;
	Buffer[position*3+1]=g;
	Buffer[position*3+2]=b;
}
void PutEdge(int x1, int y1, int x2,int y2, uint8_t r, uint8_t g, uint8_t b, GLubyte* Buffer){
	if(x1<0 || x1>959 || y1<0 || y1>539 || x2<0 || x2>959 || y2<0 || y2>539){return; }
	if(x2<x1){int i=x2;x2=x1;x1=i;i=y2;y2=y1;y1=i;}
	PutPixel(x1,y1,r,g,b,Buffer);
	int xi=0,yi=0,dx=x2-x1,dy=y2-y1;float a;
	if(dx!=0){a=(float)dy/dx;}else{a=dy;}
	while(dx!=xi || dy!=yi){
		float ca=0;
		if(xi>0){ca=(float)yi/xi;}else if(dx==0){ca=(float)yi;}else{ca=(float)yi*2;}
		if(a>=0){
			if(ca<=a && dy>yi){yi++;
			}else if(dx>xi){xi++;
			}else if(dy>yi){yi++;
		}}else{
			if(ca<a && dy<yi && dx>xi){xi++;
			}else if(dy<yi){yi--;
			}else if(dx>xi){xi++;
			}
		}
		PutPixel(x1+xi,y1+yi,r,g,b,Buffer);//printf("notnice");
	}//Display();printf("nice\n");
}
void makeCube(int x, int y, int z,double rx, double ry, double rz,int size){//treats 0 as center
	int points3D[8][3];
	for(int i=0;i<8;i++){
		int px,py,pz;
		if(i/1%2){px=size/2;}else{px=0-size/2;}
		if(i/2%2){py=size/2;}else{py=0-size/2;}
		if(i/4%2){pz=size/2;}else{pz=0-size/2;}
		double result1,result2;
		result1=px*cos((double)ry*PI/180)-pz*sin((double)ry*PI/180);
		result2=px*sin((double)ry*PI/180)+pz*cos((double)ry*PI/180);
		px=round(result1);pz=round(result2);
		points3D[i][0]=x+px;points3D[i][1]=y+py;points3D[i][2]=z+pz;
		//printf("x:%i,y:%i,z:%i\n",points3D[i][0],points3D[i][1],points3D[i][2]);
	}
	int points2D[8][2];
	for(int i=0;i<8;i++){
			points2D[i][0]=points3D[i][0]*size/points3D[i][2]+480;
			points2D[i][1]=points3D[i][1]*size/points3D[i][2]+270;
			//printf("x:%i,y:%i\n",points2D[i][0],points2D[i][1]);
	}
	for(int i=0;i<4;i++){
		PutEdge(points2D[2*i][0],points2D[2*i][1],points2D[2*i+1][0],points2D[2*i+1][1],255,0,0,PixelBuffer);
		PutEdge(points2D[i][0],points2D[i][1],points2D[i+4][0],points2D[i+4][1],0,255,0,PixelBuffer);
	}
	for(int i=0;i<2;i++){
		PutEdge(points2D[4*i][0],points2D[4*i][1],points2D[4*i+2][0],points2D[4*i+2][1],0,0,255,PixelBuffer);
		PutEdge(points2D[4*i+1][0],points2D[4*i+1][1],points2D[4*i+3][0],points2D[4*i+3][1],0,0,255,PixelBuffer);
	}
	for(int i=0;i<8;i++){PutPixel(points2D[i][0],points2D[i][1],255,255,255,PixelBuffer);}
}
void Display(){
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawPixels(960,540,GL_RGB, GL_UNSIGNED_BYTE, PixelBuffer);
	glFlush();
}


int main(int argc, char *argv){
	glutInit(&argc,&argv);
	glutInitWindowPosition(480,270);
	glutInitWindowSize(960,540);
	glutInitDisplayMode(GLUT_RGB);
	glutCreateWindow("CUBE");
	double  rotx=0,roty=0,rotz=0;
	while(roty<360){
		for(int i=0;i<1555200;i++){PixelBuffer[i]=0;}
		makeCube(0,0,100,rotx,roty,rotz,100);
		printf("rotation: x:%lf,y:%lf,z:%lf\n",rotx,roty,rotz);
		roty+=1;
		if(roty>=360){roty=0;}
		Display();
		clock_t time=clock()+CLOCKS_PER_SEC/60;
		while(clock()<time)continue;
	}
	glutDisplayFunc(Display);
	glutMainLoop();
	return 0;
}
