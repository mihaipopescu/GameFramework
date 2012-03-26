/*! 
 *  \brief     ImageFile class.
 *  \details   This class is responsible for loading / drawing bitmap images.
 *  \author    Mihai Popescu
 *  \date      01/03/2009
 *  \copyright GNU Public License.
 */

#ifndef IMAGE_FILE_H
#define IMAGE_FILE_H
#include "main.h"


enum EColorChannel
{
	ECC_RED,
	ECC_GREEN,
	ECC_BLUE,
	ECC_HUE,
	ECC_SATURATION,
	ECC_LUMINOSITY,
	ECC_EXCLUSIVERED,
	ECC_EXCLUSIVEGREEN,
	ECC_EXCLUSIVEBLUE
};


class CImageFile
{
public:
	CImageFile(void);
	virtual ~CImageFile(void);

	bool LoadBitmapFromFile(const char* szFileName, HDC hdc);
	virtual void Paint(HDC hdc, int x, int y);

	LONG Height() const { return myHeight; }
	LONG Width() const { return myWidth; }

	void Clear() { ZeroMemory(myRGBArray, sizeof(RGBQUAD) * myWidth * myHeight); }
	void Reload(HDC hdc);

	BYTE* CopyMonoImage(EColorChannel chn, const RECT* rc = NULL);
	void PasteMonoImage(const BYTE *img, EColorChannel chn, const RECT* rc = NULL);

protected:
	BITMAPINFOHEADER myBMPInfo;
	RGBQUAD *myRGBArray;
	HBITMAP myBitmap;

	LONG &myHeight;
	LONG &myWidth;
	char myFileName[MAX_PATH];
};

#endif // IMAGE_FILE_H