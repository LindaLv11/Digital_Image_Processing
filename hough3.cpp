#include <stdio.h>
#include "BmpRot.h"
#include "stdlib.h"
#include "math.h"
#include <iostream>

#define PI 3.14//Բ���ʺ궨��
#define LENGTH_NAME_BMP 30//bmpͼƬ�ļ�������󳤶�

using namespace std;
int angle[501][20000]={0};//�����ȫ��

//��������
BITMAPFILEHEADER strHead;
RGBQUAD strPla[256];//256ɫ��ɫ��
BITMAPINFOHEADER strInfo;

//��ʾλͼ�ļ�ͷ��Ϣ
void showBmpHead(BITMAPFILEHEADER pBmpHead){
	cout<<"λͼ�ļ�ͷ:"<<endl;
	cout<<"�ļ���С:"<<pBmpHead.bfSize<<endl;
	cout<<"������_1:"<<pBmpHead.bfReserved1<<endl;
	cout<<"������_2:"<<pBmpHead.bfReserved2<<endl;
	cout<<"ʵ��λͼ���ݵ�ƫ���ֽ���:"<<pBmpHead.bfOffBits<<endl<<endl;
}

void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead){
	cout<<"λͼ��Ϣͷ:"<<endl;
	cout<<"�ṹ��ĳ���:"<<pBmpInforHead.biSize<<endl;
	cout<<"λͼ��:"<<pBmpInforHead.biWidth<<endl;
	cout<<"λͼ��:"<<pBmpInforHead.biHeight<<endl;
	cout<<"biPlanesƽ����:"<<pBmpInforHead.biPlanes<<endl;
	cout<<"biBitCount������ɫλ��:"<<pBmpInforHead.biBitCount<<endl;
	cout<<"ѹ����ʽ:"<<pBmpInforHead.biCompression<<endl;
	cout<<"biSizeImageʵ��λͼ����ռ�õ��ֽ���:"<<pBmpInforHead.biSizeImage<<endl;
	cout<<"X����ֱ���:"<<pBmpInforHead.biXPelsPerMeter<<endl;
	cout<<"Y����ֱ���:"<<pBmpInforHead.biYPelsPerMeter<<endl;
	cout<<"ʹ�õ���ɫ��:"<<pBmpInforHead.biClrUsed<<endl;
	cout<<"��Ҫ��ɫ��:"<<pBmpInforHead.biClrImportant<<endl;
}


int main(){
	char strFile[LENGTH_NAME_BMP];//bmp�ļ���
	IMAGEDATA *imagedata = NULL;//��̬����洢ԭͼƬ��������Ϣ�Ķ�ά����
	IMAGEDATA *imagedataRot = NULL;//��̬����洢��ת���ͼƬ��������Ϣ�Ķ�ά����
	int width,height;//ͼƬ�Ŀ�Ⱥ͸߶�
	cout<<"��������Ҫ��ȡ���ļ���:"<<endl;
	cin>>strFile;
	FILE *fpi,*fpw;
	fpi=fopen(strFile,"rb");
	if(fpi != NULL){
		//�ȶ�ȡ�ļ�����
		WORD bfType;
		fread(&bfType,1,sizeof(WORD),fpi);
		if(0x4d42!=bfType)
		{
			cout<<"the file is not a bmp file!"<<endl;
			return NULL;
		}
		//��ȡbmp�ļ����ļ�ͷ����Ϣͷ
		fread(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpi);
		//showBmpHead(strHead);//��ʾ�ļ�ͷ
		fread(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpi);
		//showBmpInforHead(strInfo);//��ʾ�ļ���Ϣͷ

		//��ȡ��ɫ��
		for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)
		{
			fread((char *)&(strPla[nCounti].rgbBlue),1,sizeof(BYTE),fpi);
			fread((char *)&(strPla[nCounti].rgbGreen),1,sizeof(BYTE),fpi);
			fread((char *)&(strPla[nCounti].rgbRed),1,sizeof(BYTE),fpi);
			fread((char *)&(strPla[nCounti].rgbReserved),1,sizeof(BYTE),fpi);
		}

		width = strInfo.biWidth;
		height = strInfo.biHeight;
		//ͼ��ÿһ�е��ֽ���������4��������
		width = (width * sizeof(IMAGEDATA) + 3) / 4 * 4;
		//imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));
		imagedata = (IMAGEDATA*)malloc(width * height);
		imagedataRot = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));
		//��ʼ��ԭʼͼƬ����������
		for(int i = 0;i < height;++i)
		{
			for(int j = 0;j < width;++j)
			{
				(*(imagedata + i * width + j)).blue = 0;
				//(*(imagedata + i * width + j)).green = 0;
				//(*(imagedata + i *  width + j)).red = 0;
			}
		}
		//��ʼ��ƽ����ͼƬ����������
		for(int i = 0;i < height;++i)
		{
			for(int j = 0;j < width;++j)
			{
				(*(imagedataRot + i * width + j)).blue = 0;
				//(*(imagedataRot + i * 2 * width + j)).green = 0;
				//(*(imagedataRot + i * 2 * width + j)).red = 0;
			}
		}
		//fseek(fpi,54,SEEK_SET);
		//����ͼƬ����������
		fread(imagedata,sizeof(struct tagIMAGEDATA) * width,height,fpi);
		fclose(fpi);
	}
	else
	{
		cout<<"file open error!"<<endl;
		return NULL;
	}

	//ͼƬ��Ե���
     int o,g,w;
     int ph[3][3]={-1,-2,-1,0,0,0,1,2,1};
     int phv[3][3]={-1,0,1,-2,0,2,-1,0,1};
    	for(int i = 1;i <(height-1);++i)
	{
		for(int j = 1;j <(width-1);++j)
		{
		    o=0;
		    for (int m=-1;m<=1;m++)
            {
                for (int n=-1;n<=1;n++)
                {
                o=((*(imagedata + (i+m) *  width + (j+n))).blue)*ph[m+1][n+1]+o;
                }
            }
            o=fabs(o);
            g=0;
            for (int m=-1;m<=1;m++)
            {
                for(int n=-1;n<=1;n++)
                {
                    g=((*(imagedata + (i+m) *  width + (j+n))).blue)*phv[m+1][n+1]+g;
                }
            }
            g=fabs(g);
            w=o+g;
            if(w>255)
            {
                w=255;
            }

            (*(imagedataRot + i *  width +j)).blue=w;
        }
	}
     for(int i = 1;i < height-1;++i)
		{
			for(int j = 1;j < width-1;++j)
			{
				if((*(imagedataRot + i * width + j)).blue <=200)
                {(*(imagedataRot + i * width + j)).blue=0;}
                 else
                {
                   (*(imagedataRot + i * width + j)).blue=255;
                }
			}
		}
   //hough�任  ֮ǰ�������в����������ֿ��ܣ�
   //һ�ǿ���angle[501][20000]={0};������˾ֲ����������¿ռ䲻��
   //���ǿ���ѭ��Ƕ��̫�࣬���Ĳ��Ժ�������������Ӧ���ù�ʽ������ȥһ��ѭ��
    int d=(int)(sqrt(height*height+width*width)+0.5);//����
    int ang=501;
    double hough_interval=PI/(double)(ang-1);//��PI��Ϊanl��
    int centerX=width/2,centerY=height/2;//ͼ������

    //i*cost+j*sint=q����t,i,j��q��
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            if((*(imagedataRot+i*width+j)).blue==0)continue;//�ڵ��������沽��ֱ�ӽ���j++
            for(int t=0;t<ang;t++)
            {
                    int q=j*cos(hough_interval*t)+i*sin(hough_interval*t)+0.5;//��������
                    q=q+d;//�����п����Ǹ��ģ�����Ҫ��0��2d���壬����q=q+d.
                    if(q>=0&&q<2*d)
                        { angle[t][q]+=1;}//qԽ��Ļ�ֱ��j++�������ͶƱ

            }
        }
    }
//�Ȱ�ԭͼ����imagedataRot
                for(int i=0;i<height;i++)
                {
                  for(int j=0;j<width;j++)
                    {
                        (*(imagedataRot+i*width+j)).blue=(*(imagedata+i*width+j)).blue;
                    }
                }
//���ͶƱ��
int maxl=0;
for(int row = 0; row < ang; row++)
    {
    for(int col = 0; col < 2*d; col++)
     {
            if(angle[row][col] > maxl)
maxl=angle[row][col];
    }
}
//Ϊ�˻�����ֻһ��ֱ�ߣ�ȡ0.6����maxl
int maxline=0.6*(double)maxl;
for(int row = 0; row < ang; row++)
{
    for(int col = 0; col < 2*d; col++)
     {
        if(angle[row][col] < maxline) continue;//���ڵ��ڵ���������
  //������
        double dy = sin(row * hough_interval);
        double dx = cos(row* hough_interval);
         for (int i = 0; i < height; ++i)
          {
              int j =((col - d - (i * dy)) / dx)  +0.5;//�ù�ʽ��������һ��ѭ��
              if ((j < width) && (j >= 0))
              {
                  (*(imagedataRot+i*width+j)).blue=255;
              }
          }
            for (int j = 0; j < width; ++j)
             {
              int i = ((col - d- (j* dx)) / dy)+0.5;
              if ((i < height) && (i >= 0))
              {
                   (*(imagedataRot+i*width+j)).blue=255;
              }
            }
    }
}

	//����bmpͼƬ
	if((fpw=fopen("result.bmp","wb"))==NULL)
	{
		cout<<"create the bmp file error!"<<endl;
		return NULL;
	}
	WORD bfType_w=0x4d42;
	fwrite(&bfType_w,1,sizeof(WORD),fpw);
	//fpw +=2;
	fwrite(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpw);
	strInfo.biWidth = width;
	strInfo.biHeight = height;
	fwrite(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpw);
	//�����ɫ������
	for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)
	{
		fwrite(&strPla[nCounti].rgbBlue,1,sizeof(BYTE),fpw);
		fwrite(&strPla[nCounti].rgbGreen,1,sizeof(BYTE),fpw);
		fwrite(&strPla[nCounti].rgbRed,1,sizeof(BYTE),fpw);
		fwrite(&strPla[nCounti].rgbReserved,1,sizeof(BYTE),fpw);
	}
	//������������
	for(int i =0;i < height;++i)
	{
		for(int j = 0;j < width;++j)
		{
			fwrite( &((*(imagedataRot + i * width + j)).blue),1,sizeof(BYTE),fpw);
			//fwrite( &((*(imagedataRot + i * 2 * width + j)).green),1,sizeof(BYTE),fpw);
			//fwrite( &((*(imagedataRot + i * 2 * width + j)).red),1,sizeof(BYTE),fpw);
		}
	}
	fclose(fpw);

	//�ͷ��ڴ�
	delete[] imagedata;
	delete[] imagedataRot;
}
