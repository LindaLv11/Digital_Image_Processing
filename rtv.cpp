#include <stdio.h>
#include "BmpRot.h"
#include "stdlib.h"
#include "math.h"
#include <iostream>
#include <string>
using namespace std;

#define PI 3.14159//Բ���ʺ궨��
#define LENGTH_NAME_BMP 30//bmpͼƬ�ļ�������󳤶�

using namespace std;

double ph[3][3]={-1,-2,-1,0,0,0,1,2,1};
 double phv[3][3]={-1,0,1,-2,0,2,-1,0,1};
   double o[600][600]={0};
   double g[600][600]={0};
    double fo[600][600]={0};
   double fg[600][600]={0};
   double faix[600][600]={0};
    double faiy[600][600]={0};
  double pusaix[600][600]={0};
  double pusaiy[600][600]={0};
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
		imagedataRot = (IMAGEDATA*)malloc( width *  height * sizeof(IMAGEDATA));
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
		//��ʼ����ת��ͼƬ����������
		for(int i = 0;i <height;++i)
		{
			for(int j = 0;j <  width;++j)
			{
				(*(imagedataRot + i *  width + j)).blue = 0;
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

	//rtv
     int i,j,m,n;
     double q;
     double z;
     //ÿ�����soble����x
    	for( i = 2;i < (height-2);++i)
	{
		for(j = 2;j <  (width-2);++j)
		{
		    o[i][j]=0;
		    for ( m=(-1);m<=1;m++)
            {
                for ( n=(-1);n<=1;n++)
                {
                o[i][j]=((*(imagedata + (i+m) *  width + (j+n))).blue)*ph[m+1][n+1]+o[i][j];
                }
            }
            fo[i][j]=fabs(o[i][j]);
		}
    }


    ////ÿ�����sobel����y
    	for( i = 2;i < (height-2);++i)
	{
		for( j = 2;j <  (width-2);++j)
		{
            g[i][j]=0;
            for ( m=(-1);m<=1;m++)
            {
                for( n=(-1);n<=1;n++)
                {
                    g[i][j]=((*(imagedata + (i+m) *  width + (j+n))).blue)*phv[m+1][n+1]+g[i][j];
                }
            }
            fg[i][j]=fabs(g[i][j]);
        }
	}
	////fai
    	for( i = 2;i < (height-2);++i)
	{
		for(j = 2;j <  (width-2);++j)
		{
             faix[i][j]=0;
		     for ( m=(-2);m<=2;m++)
            {
                for ( n=(-2);n<=2;n++)
                {

                    q=-((m*m+n*n)/2.00);
                    q=(double)exp(q);
                    faix[i][j]=faix[i][j]+q*fo[i+m][j+n];
                }
            }

		}
	}
        for( i = 2;i < (height-2);++i)
	{
		for(j = 2;j <  (width-2);++j)
		{
            faiy[i][j]=0;
		     for ( m=(-2);m<=2;m++)
            {
                for ( n=(-2);n<=2;n++)
                {
                    q=-((m*m)+(n*n)/2.0);
                    q=(double)exp(q);
                    faiy[i][j]=faiy[i][j]+q*fg[i+m][j+n];
                }
            }

		}
	}

        for( i = 2;i < (height-2);++i)
	{
		for(j = 2;j <  (width-2);++j)
		{
           pusaix[i][j]=0;
		     for ( m=(-2);m<=2;m++)
            {
                for ( n=(-2);n<=2;n++)
                {
                    q=-((m*m+n*n)/2.00);
                    q=(double)exp(q);
                    pusaix[i][j]=pusaix[i][j]+q*o[i+m][j+n];
                }
            }
            pusaix[i][j]=fabs(pusaix[i][j]);
		}
	}
        for( i = 2;i < (height-2);++i)
	{
		for(j = 2;j <  (width-2);++j)
		{
            pusaiy[i][j]=0;
		     for ( m=(-2);m<=2;m++)
            {
                for ( n=(-2);n<=2;n++)
                {
                    q=-((m*m+n*n)/2.00);
                    q=(double)exp(q);
                    pusaiy[i][j]=pusaiy[i][j]+q*g[i+m][j+n];
                }
            }
             pusaiy[i][j]=fabs(pusaiy[i][j]);
		}
	}

	////////////////////////////

     for(i = 2;i < height-2;++i)
		{
			for( j = 2;j < width-2;++j)
			{
            z=0;
            z=(faix[i][j]/(pusaix[i][j]+1))+(faiy[i][j]/(pusaiy[i][j]+1));
            if(z<=255&&z>=0)
            {(*(imagedataRot + i * width + j)).blue=z;}
            else
            {(*(imagedataRot + i * width + j)).blue=255;}
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
	strInfo.biWidth =  width;
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
	for(int i =0;i <  height;++i)
	{
		for(int j = 0;j <  width;++j)
		{
			fwrite( &((*(imagedataRot + i *  width + j)).blue),1,sizeof(BYTE),fpw);
			//fwrite( &((*(imagedataRot + i * 2 * width + j)).green),1,sizeof(BYTE),fpw);
			//fwrite( &((*(imagedataRot + i * 2 * width + j)).red),1,sizeof(BYTE),fpw);
		}
	}
	fclose(fpw);

	//�ͷ��ڴ�
	delete[] imagedata;
	delete[] imagedataRot;
}
