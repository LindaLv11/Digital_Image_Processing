#include "BmpRot.h"
#include "stdlib.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define PI 3.14159//圆周率宏定义
#define LENGTH_NAME_BMP 30//bmp图片文件名的最大长度

using namespace std;

//变量定义
BITMAPFILEHEADER strHead;
RGBQUAD strPla[256];//256色调色板
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
//显示位图文件头信息
void showBmpHead(BITMAPFILEHEADER pBmpHead){
	cout<<"位图文件头:"<<endl;
	cout<<"文件大小:"<<pBmpHead.bfSize<<endl;
	cout<<"保留字_1:"<<pBmpHead.bfReserved1<<endl;
	cout<<"保留字_2:"<<pBmpHead.bfReserved2<<endl;
	cout<<"实际位图数据的偏移字节数:"<<pBmpHead.bfOffBits<<endl<<endl;
}

void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead){
	cout<<"位图信息头:"<<endl;
	cout<<"结构体的长度:"<<pBmpInforHead.biSize<<endl;
	cout<<"位图宽:"<<pBmpInforHead.biWidth<<endl;
	cout<<"位图高:"<<pBmpInforHead.biHeight<<endl;
	cout<<"biPlanes平面数:"<<pBmpInforHead.biPlanes<<endl;
	cout<<"biBitCount采用颜色位数:"<<pBmpInforHead.biBitCount<<endl;
	cout<<"压缩方式:"<<pBmpInforHead.biCompression<<endl;
	cout<<"biSizeImage实际位图数据占用的字节数:"<<pBmpInforHead.biSizeImage<<endl;
	cout<<"X方向分辨率:"<<pBmpInforHead.biXPelsPerMeter<<endl;
	cout<<"Y方向分辨率:"<<pBmpInforHead.biYPelsPerMeter<<endl;
	cout<<"使用的颜色数:"<<pBmpInforHead.biClrUsed<<endl;
	cout<<"重要颜色数:"<<pBmpInforHead.biClrImportant<<endl;
}
	double compInc(const void *a, const void *b)
   {
    return *(double *)a - *(double *)b;
   }

int main(){
	char strFile[LENGTH_NAME_BMP];//bmp文件名
	IMAGEDATA *imagedata = NULL;//动态分配存储原图片的像素信息的二维数组
	IMAGEDATA *imagedataRot = NULL;//动态分配存储旋转后的图片的像素信息的二维数组
	int width,height;//图片的宽度和高度
	cout<<"请输入所要读取的文件名:"<<endl;
	cin>>strFile;
	FILE *fpi,*fpw;
	fpi=fopen(strFile,"rb");
	if(fpi != NULL){
		//先读取文件类型
		WORD bfType;
		fread(&bfType,1,sizeof(WORD),fpi);
		if(0x4d42!=bfType)
		{
			cout<<"the file is not a bmp file!"<<endl;
		return NULL;
		}
		//读取bmp文件的文件头和信息头
		fread(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpi);
		//showBmpHead(strHead);//显示文件头
		fread(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpi);
		//showBmpInforHead(strInfo);//显示文件信息头

		//读取调色板
		for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)
		{
			fread((char *)&(strPla[nCounti].rgbBlue),1,sizeof(BYTE),fpi);
			fread((char *)&(strPla[nCounti].rgbGreen),1,sizeof(BYTE),fpi);
			fread((char *)&(strPla[nCounti].rgbRed),1,sizeof(BYTE),fpi);
			fread((char *)&(strPla[nCounti].rgbReserved),1,sizeof(BYTE),fpi);
		}

		width = strInfo.biWidth;
		height = strInfo.biHeight;
		//图像每一行的字节数必须是4的整数倍
		width = (width * sizeof(IMAGEDATA) + 3) / 4 * 4;
		//imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));
		imagedata = (IMAGEDATA*)malloc(width * height);
		imagedataRot = (IMAGEDATA*)malloc(2 * width * 2 * height * sizeof(IMAGEDATA));
		//初始化原始图片的像素数组
		for(int i = 0;i < height;++i)
		{
			for(int j = 0;j < width;++j)
			{
				(*(imagedata + i * width + j)).blue = 0;
				//(*(imagedata + i * width + j)).green = 0;
				//(*(imagedata + i *  width + j)).red = 0;
			}
		}
		//初始化旋转后图片的像素数组
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
		//读出图片的像素数据
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
	//保存bmp图片
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
	//保存调色板数据
	for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)
	{
		fwrite(&strPla[nCounti].rgbBlue,1,sizeof(BYTE),fpw);
		fwrite(&strPla[nCounti].rgbGreen,1,sizeof(BYTE),fpw);
		fwrite(&strPla[nCounti].rgbRed,1,sizeof(BYTE),fpw);
		fwrite(&strPla[nCounti].rgbReserved,1,sizeof(BYTE),fpw);
	}
	//保存像素数据
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

	//释放内存
	delete[] imagedata;
	delete[] imagedataRot;
}


