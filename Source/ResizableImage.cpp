#include "ResizableImage.h"


void CResizableImage::ScaleRow(unsigned int dst_width, unsigned int /*dst_height*/, unsigned int row)
{
	RGBQUAD *pDstRow = &(myResizedImage[row * dst_width]);
	RGBQUAD *pSrcRow = &(myRGBArray[row * myWidth]);

	for (UINT x = 0; x < dst_width; x++) 
	{
		// Loop through row
		BYTE r = 0;
		BYTE g = 0;
		BYTE b = 0;
		int iLeft = myWeightsTable->getLeftBoundary(x);	// Retrieve left boundaries
		int iRight = myWeightsTable->getRightBoundary(x);  // Retrieve right boundaries
		for (int i = iLeft; i <= iRight; i++)
		{
			// Scan between boundaries
			// Accumulate weighted effect of each neighboring pixel
			r += (BYTE)(myWeightsTable->getWeight(x, i-iLeft) * (double)(pSrcRow[i].rgbRed)); 
			g += (BYTE)(myWeightsTable->getWeight(x, i-iLeft) * (double)(pSrcRow[i].rgbGreen)); 
			b += (BYTE)(myWeightsTable->getWeight(x, i-iLeft) * (double)(pSrcRow[i].rgbBlue)); 
		} 
		// set destination row
		pDstRow[x].rgbRed = r;
		pDstRow[x].rgbGreen = g;
		pDstRow[x].rgbBlue = b;
		pDstRow[x].rgbReserved = 0;
	}
}

void CResizableImage::HorizontalFilter(unsigned int dst_width, unsigned int dst_height)
{

	if (dst_width == myWidth)
	{
		// No scaling required, just copy
		memcpy (myResizedImage, myRGBArray, sizeof(RGBQUAD) * myWidth * myHeight);
	}

	myWeightsTable = new CWeightsTable(myFilter, dst_width, myWidth);

	for (UINT u = 0; u < dst_height; u++)
	{
		// scale each row
		ScaleRow (dst_width, dst_width, u);	// Scale each row 
	}

	delete myWeightsTable;
}

void CResizableImage::ScaleCol(unsigned int dst_width, unsigned int dst_height, unsigned int col)
{ 
	for (UINT y = 0; y < dst_height; y++) 
	{
		// Loop through column
		BYTE r = 0;
		BYTE g = 0;
		BYTE b = 0;
		int iLeft = myWeightsTable->getLeftBoundary(y);	// Retrieve left boundaries
		int iRight = myWeightsTable->getRightBoundary(y);  // Retrieve right boundaries
		for (int i = iLeft; i <= iRight; i++)
		{
			// Scan between boundaries
			// Accumulate weighted effect of each neighboring pixel
			RGBQUAD &src = myRGBArray[i * myWidth + col];
			r += (BYTE)(myWeightsTable->getWeight(y, i-iLeft) * (double)(src.rgbRed));
			g += (BYTE)(myWeightsTable->getWeight(y, i-iLeft) * (double)(src.rgbGreen));
			b += (BYTE)(myWeightsTable->getWeight(y, i-iLeft) * (double)(src.rgbBlue));
		}

		RGBQUAD &dst = myResizedImage[y * dst_width + col];
		dst.rgbRed = r;
		dst.rgbGreen = g;
		dst.rgbBlue = b;
		dst.rgbReserved = 0;
	}
}


void CResizableImage::VerticalFilter(unsigned int dst_width, unsigned int dst_height)
{
	if (myHeight == dst_height)
	{
		// No scaling required, just copy
		memcpy(myResizedImage, myRGBArray, sizeof (RGBQUAD) * myWidth * myHeight);
	}

	myWeightsTable = new CWeightsTable(myFilter, dst_height, myHeight);

	for (UINT u = 0; u < dst_width; u++)
	{
		// Step through columns
		ScaleCol(dst_width, dst_height, u);   // Scale each column
	}

	delete myWeightsTable;
}

void CResizableImage::Resample(unsigned dst_width, unsigned dst_height)
{
	// decide which filtering order (xy or yx) is faster for this mapping
	if(dst_width * myHeight <= dst_height * myWidth) 
	{
		myResizedImage = new RGBQUAD[dst_width * myHeight];

		HorizontalFilter(dst_width, myHeight);

		delete myRGBArray;
		myRGBArray = myResizedImage;
		myWidth = dst_width;
		myResizedImage = new RGBQUAD[dst_width * dst_height];

		VerticalFilter(dst_width, dst_height);
	} 
	else 
	{
		myResizedImage = new RGBQUAD[myWidth * dst_height];
		VerticalFilter(myWidth, dst_height);

		delete myRGBArray;
		myRGBArray = myResizedImage;
		myHeight = dst_height;
		myResizedImage = new RGBQUAD[dst_width * dst_height];

		HorizontalFilter(dst_width, dst_height);
	}

	delete myRGBArray;
	myRGBArray = myResizedImage;
	myWidth = dst_width;
	myHeight = dst_height;

	DeleteObject(myBitmap);
	myBitmap = 0;
}