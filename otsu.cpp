#include "BmpRot.h"
#include "stdlib.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define PI 3.14159//Բ���ʺ궨��
#define LENGTH_NAME_BMP 30//bmpͼƬ�ļ�������󳤶�

using namespace std;

//��������
BITMAPFILEHEADER strHead;
RGBQUAD strPla[256];//256ɫ��ɫ��
BITMAPINFOHEADER strInfo;
int sumq(int *a, int n)
   {
       int sum,i;
       for(i=1;i<=n;i++)
       {
           sum+=*(a+i);
       }
       return sum;
   }
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
	double compInc(const void *a, const void *b)
   {
    return *(double *)a - *(double *)b;
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
		imagedataRot = (IMAGEDATA*)malloc(2 * width * 2 * height * sizeof(IMAGEDATA));
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
		for(int i = 0;i <  height;++i)
		{
			for(int j = 0;j < width;++j)
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

	//otsu
	long p[16];
	int i,j,t;
	for(i=0;i<height;++i)
    {
        for(j=0;j<width;++j)
        {
            t=(int)((*(imagedata + i *  width + j)).blue/16);
            p[t]++;
        }
    }
    long total=height*width;
    for (i=0;i<16;i++)
    {
        p[i]=p[i]/total;
    }
    int k=0;
    long p1[16]={0};
    for (k=0;k<16;k++)
    {
        for(i=0;i<=k;i++)
        {
             p1[k]=p1[k]+p[i];
        }
    }
    long m[16]={0};
    for(k=0;k<16;k++)
    {
        for(i=0;i<=k;i++)
        {
            m[k]=m[k]+i*p[i];
        }
    }
   long sigmab[16]={0};
    for(k=0;k<16;k++)
    {
        sigmab[k]=(m[15]*p1[k]-m[k])*(m[15]*p1[k]-m[k])/(p1[k]*(1-p1[k]));
    }
    printf("%l",sigmab[1]);
    long maxk=0;
    double k1=0;
    for (k=0;k<16;k++)
    {
        if(sigmab[k]>maxk)
        {
            maxk=sigmab[k];
        }
    }
    double countm=0;
    for (k=0;k<16;k++)
    {
        if(sigmab[k]=maxk)
        {
            countm++;
            k1=k+k1;
        }
    }
    k1=k1/countm;
    for(i=0;i<height;++i)
    {
        for(j=0;j<width;++j)
        {
            if((*(imagedata + i *  width + j)).blue<=k1*16)
            {
            (*(imagedataRot + i *  width + j)).blue=0;
            }
            else
                (*(imagedataRot + i *  width + j)).blue=255;
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
	strInfo.biHeight =  height;
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


