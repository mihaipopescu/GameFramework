#include "ImageFile.h"

extern HINSTANCE g_hInst;


CImageFile::CImageFile() : myHeight(myBMPInfo.biHeight), myWidth(myBMPInfo.biWidth)
{
	myBitmap = 0;
	myRGBArray = NULL;
	ZeroMemory(&myBMPInfo, sizeof(BITMAPINFOHEADER));
}

bool CImageFile::LoadBitmapFromFile(const char *szFileName, HDC hdc)
{
	BYTE *pData;
	HDC mdc = CreateCompatibleDC(hdc);

	strcpy_s(myFileName, MAX_PATH, szFileName);

	// release previously loaded file data
	if(myRGBArray)
	{
		delete[] myRGBArray;
		myRGBArray = NULL;
	}

	if(myBitmap)
	{
		DeleteObject(myBitmap);
		myBitmap = 0;
	}

	// Loads the image.
	myBitmap = (HBITMAP)LoadImage(g_hInst, szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);	

	if(!myBitmap)
		return false;

	ZeroMemory(&myBMPInfo, sizeof(BITMAPINFO));
	myBMPInfo.biSize=sizeof(BITMAPINFOHEADER);

	// send NULL to bits to get the bitmap info
	GetDIBits(mdc, myBitmap, 0, 0, NULL, (BITMAPINFO*)&myBMPInfo, DIB_RGB_COLORS);

	pData = new BYTE[myBMPInfo.biSizeImage];

	// read the bitmap data
	// NOTE: We use this method to access the bitmap bits in order to modify them
	// applying different filters or other image processing algorithms in real time 
	// such as blur effect (denoising) or other convolutions.
	GetDIBits(mdc, myBitmap, 0, myHeight, pData, (BITMAPINFO*)&myBMPInfo, DIB_RGB_COLORS);

	myRGBArray = new RGBQUAD[myWidth * myHeight];

	// Fill RGB image on 32 bit in order to display it
	if(myBMPInfo.biBitCount == 24)
	{
		int size = myWidth * myHeight;
		unsigned char *data = pData;
		RGBQUAD *c = myRGBArray;

		for(int i=0;i<size;i++)
		{
			c->rgbBlue = *data++;
			c->rgbGreen = *data++;
			c->rgbRed = *data++;
			c->rgbReserved = 0;
			c++;
		}
	}

	myBMPInfo.biBitCount = 32;

	DeleteObject(myBitmap);
	myBitmap = 0;

	DeleteDC(mdc);

	delete[] pData;

	return true;
}

void CImageFile::Reload(HDC hdc)
{
	LoadBitmapFromFile(myFileName, hdc);
}

void CImageFile::Paint(HDC hdc, int x, int y)
{
	if(!myRGBArray)
		return;

	if(!myBitmap)
		myBitmap = CreateCompatibleBitmap(hdc, myWidth, myHeight);

	HDC mdc = CreateCompatibleDC(hdc);

	SelectObject(mdc, myBitmap);

	SetDIBits(mdc, myBitmap, 0, myHeight, myRGBArray, (BITMAPINFO*)&myBMPInfo, DIB_RGB_COLORS);

	BitBlt(hdc, x, y, myWidth, myHeight, mdc, 0, 0, SRCCOPY);

	DeleteDC(mdc);
}


CImageFile::~CImageFile(void)
{
	if(myRGBArray)
		delete[] myRGBArray;

	DeleteObject(myBitmap);
}

BYTE* CImageFile::CopyMonoImage(EColorChannel chn, const RECT* rc)
{
	int imgHeight = rc? rc->bottom - rc->top + 1 : myHeight;
	int imgWidth = rc? rc->right - rc->left + 1 : myWidth;
	int x = rc? rc->left : 0;
	int y = rc? rc->top : 0;

	BYTE *img = new BYTE[imgHeight * imgWidth];

	switch(chn)
	{

	case ECC_RED:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
				img[i*imgWidth + j] = myRGBArray[(i+y)*myWidth + j + x].rgbRed;
		break;

	case ECC_GREEN:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
				img[i*imgWidth + j] = myRGBArray[(i+y)*myWidth + j + x].rgbGreen;
		break;

	case ECC_BLUE:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
				img[i*imgWidth + j] = myRGBArray[(i+y)*myWidth + j + x].rgbBlue;
		break;

	case ECC_HUE:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
			{
				RGBQUAD &q = myRGBArray[(i+y)*myWidth + j + x];
				float r = q.rgbRed/255.0f;
				float g = q.rgbGreen/255.0f;
				float b = q.rgbBlue/255.0f;

				float u = max(r, g);
				u = max(b, u);
				float d = min(r, g);
				d = min(b, d);

				BYTE v;

				if(fabsf(u-d)<EPS)
					v=0;
				else
				{
					float f = 1/(u-d);

					if(fabsf(u-r)<EPS)
					{
						f *= (g-b)*60.f;
					}
					else
					if(fabsf(u-g)<EPS)
					{
						f *= (b-r)*60.f;
						f += 120;
					}
					else
					if(fabsf(u-b)<EPS)
					{
						f *= (r-g)*60.f;
						f += 240;
					}

					v = (BYTE)(f*255.f/360.f);
				}

				img[i*imgWidth + j] = v;
			}

	case ECC_SATURATION:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
			{
				RGBQUAD &q = myRGBArray[(i+y)*myWidth + j + x];
				float r = q.rgbRed/255.0f;
				float g = q.rgbGreen/255.0f;
				float b = q.rgbBlue/255.0f;

				float u = max(r, g);
				u = max(b, u);
				float d = min(r, g);
				d = min(b, d);

				BYTE v;

				if(fabsf(u-d)<EPS)
					v=0;
				else
				{
					float l = (u+d)/2;
					float f = (u-d);

					if(l<=0.5f)
						f /= u+d;
					else
						f /= 2-u-d;

					v = (BYTE)(f*255.f);
				}

				img[i*imgWidth + j] = v;
			}
		break;

	case ECC_LUMINOSITY:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
			{
				RGBQUAD &q = myRGBArray[(i+y)*myWidth + j + x];
				float r = q.rgbRed/255.0f;
				float g = q.rgbGreen/255.0f;
				float b = q.rgbBlue/255.0f;

				float u = max(r, g);
				u = max(b, u);
				float d = min(r, g);
				d = min(b, d);

				BYTE v;

				if(fabsf(u-d)<EPS)
					v=0;
				else
				{
					float f = (u+d)/2;
					v = (BYTE)(f*255.f);
				}

				img[i*imgWidth + j] = v;
			}
		break;
	}

	return img;
}

void CImageFile::PasteMonoImage(const BYTE *img, EColorChannel chn, const RECT* rc)
{
	int imgHeight = rc? rc->bottom - rc->top + 1 : myHeight;
	int imgWidth = rc? rc->right - rc->left + 1 : myWidth;
	int x = rc? rc->left : 0;
	int y = rc? rc->top : 0;

	if(chn >= ECC_EXCLUSIVERED)
		Clear();

	switch(chn)
	{

	case ECC_EXCLUSIVERED:
	case ECC_RED:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
				myRGBArray[(i+y)*myWidth + j + x].rgbRed = img[i*imgWidth + j];
		break;

	case ECC_EXCLUSIVEGREEN:
	case ECC_GREEN:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
				myRGBArray[(i+y)*myWidth + j + x].rgbGreen = img[i*imgWidth + j];
		break;

	case ECC_EXCLUSIVEBLUE:
	case ECC_BLUE:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
				myRGBArray[(i+y)*myWidth + j + x].rgbBlue = img[i*imgWidth + j];
		break;
	}

}

