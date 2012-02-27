#include "ResizeEngine.h"

CWeightsTable::CWeightsTable(CGenericFilter *pFilter, DWORD uDstSize, DWORD uSrcSize) 
{
	DWORD u;
	double dWidth;
	double dFScale = 1.0;
	double dFilterWidth = pFilter->GetWidth();

	// scale factor
	double dScale = double(uDstSize) / double(uSrcSize);

	if(dScale < 1.0) 
	{
		// minification
		dWidth = dFilterWidth / dScale;
		dFScale = dScale;
	} 
	else 
	{
		// magnification
		dWidth= dFilterWidth;
	}

	// allocate a new line contributions structure
	// window size is the number of sampled pixels
	m_WindowSize = 2 * (int)ceil(dWidth) + 1;
	m_LineLength = uDstSize;
	// allocate list of contributions
	m_WeightTable = new sContribution[m_LineLength];
	for(u = 0 ; u < m_LineLength ; u++) 
	{
		// allocate contributions for every pixel
		m_WeightTable[u].Weights = new double[m_WindowSize];
	}

	for(u = 0; u < m_LineLength; u++) 
	{
		// scan through line of contributions
		double dCenter = (double)u / dScale;   // reverse mapping
		// find the significant edge points that affect the pixel
		int iLeft = max(0, (int)floor(dCenter - dWidth));
		int iRight = min((int)ceil(dCenter + dWidth), int(uSrcSize) - 1);

		// cut edge points to fit in filter window in case of spill-off
		if((iRight - iLeft + 1) > int(m_WindowSize)) 
		{
			if(iLeft < (int(uSrcSize) - 1 / 2)) 
			{
				iLeft++;
			} 
			else 
			{
				iRight--;
			}
		}

		m_WeightTable[u].Left = iLeft;
		m_WeightTable[u].Right = iRight;

		int iSrc = 0;
		double dTotalWeight = 0;  // zero sum of weights
		for(iSrc = iLeft; iSrc <= iRight; iSrc++) 
		{
			// calculate weights
			double weight = dFScale * pFilter->Filter(dFScale * (dCenter - (double)iSrc));
			m_WeightTable[u].Weights[iSrc-iLeft] = weight;
			dTotalWeight += weight;
		}

		if(dTotalWeight > 0) 
		{
			// normalize weight of neighbouring points
			for(iSrc = iLeft; iSrc <= iRight; iSrc++)
			{
				// normalize point
				m_WeightTable[u].Weights[iSrc-iLeft] /= dTotalWeight;
			}
		}
	}
}

CWeightsTable::~CWeightsTable() 
{
		for(DWORD u = 0; u < m_LineLength; u++) 
		{
				// free contributions for every pixel
				delete []m_WeightTable[u].Weights;
		}

		// free list of pixels contributions
		delete []m_WeightTable;
}


void CResizableImage::ScaleRow(unsigned int dst_width, unsigned int /*dst_height*/, unsigned int row)
{
	RGBQUAD *pDstRow = &(m_pResImg[row * dst_width]);
	RGBQUAD *pSrcRow = &(m_pRGB[row * width]);

	for (UINT x = 0; x < dst_width; x++) 
	{
		// Loop through row
		BYTE r = 0;
		BYTE g = 0;
		BYTE b = 0;
		int iLeft = m_pWeights->getLeftBoundary(x);	// Retrieve left boundries
		int iRight = m_pWeights->getRightBoundary(x);  // Retrieve right boundries
		for (int i = iLeft; i <= iRight; i++)
		{
			// Scan between boundries
			// Accumulate weighted effect of each neighboring pixel
			r += (BYTE)(m_pWeights->getWeight(x, i-iLeft) * (double)(pSrcRow[i].rgbRed)); 
			g += (BYTE)(m_pWeights->getWeight(x, i-iLeft) * (double)(pSrcRow[i].rgbGreen)); 
			b += (BYTE)(m_pWeights->getWeight(x, i-iLeft) * (double)(pSrcRow[i].rgbBlue)); 
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

	if (dst_width == width)
	{
		// No scaling required, just copy
		memcpy (m_pResImg, m_pRGB, sizeof(RGBQUAD) * width * height);
	}
	
	m_pWeights = new CWeightsTable(m_pFilter, dst_width, width);

	for (UINT u = 0; u < dst_height; u++)
	{
		// scale each row
		ScaleRow (dst_width, dst_width, u);	// Scale each row 
	}

	delete m_pWeights;
}

void CResizableImage::ScaleCol(unsigned int dst_width, unsigned int dst_height, unsigned int col)
{ 
	for (UINT y = 0; y < dst_height; y++) 
	{
		// Loop through column
		BYTE r = 0;
		BYTE g = 0;
		BYTE b = 0;
		int iLeft = m_pWeights->getLeftBoundary(y);	// Retrieve left boundries
		int iRight = m_pWeights->getRightBoundary(y);  // Retrieve right boundries
		for (int i = iLeft; i <= iRight; i++)
		{
			// Scan between boundries
			// Accumulate weighted effect of each neighboring pixel
			RGBQUAD &src = m_pRGB[i * width + col];
			r += (BYTE)(m_pWeights->getWeight(y, i-iLeft) * (double)(src.rgbRed));
			g += (BYTE)(m_pWeights->getWeight(y, i-iLeft) * (double)(src.rgbGreen));
			b += (BYTE)(m_pWeights->getWeight(y, i-iLeft) * (double)(src.rgbBlue));
		}

		RGBQUAD &dst = m_pResImg[y * dst_width + col];
		dst.rgbRed = r;
		dst.rgbGreen = g;
		dst.rgbBlue = b;
		dst.rgbReserved = 0;
	}
}


void CResizableImage::VerticalFilter(unsigned int dst_width, unsigned int dst_height)
{
	if (height == dst_height)
	{
		// No scaling required, just copy
		memcpy(m_pResImg, m_pRGB, sizeof (RGBQUAD) * width * height);
	}
	
	m_pWeights = new CWeightsTable(m_pFilter, dst_height, height);

	for (UINT u = 0; u < dst_width; u++)
	{
		// Step through columns
		ScaleCol(dst_width, dst_height, u);   // Scale each column
	}

	delete m_pWeights;
}

void CResizableImage::Resample(unsigned dst_width, unsigned dst_height)
{
	// decide which filtering order (xy or yx) is faster for this mapping
	if(dst_width * height <= dst_height * width) 
	{
		m_pResImg = new RGBQUAD[dst_width * height];

		HorizontalFilter(dst_width, height);
		
		delete m_pRGB;
		m_pRGB = m_pResImg;
		width = dst_width;
		m_pResImg = new RGBQUAD[dst_width * dst_height];

		VerticalFilter(dst_width, dst_height);
	} 
	else 
	{
		m_pResImg = new RGBQUAD[width * dst_height];
		VerticalFilter(width, dst_height);
		
		delete m_pRGB;
		m_pRGB = m_pResImg;
		height = dst_height;
		m_pResImg = new RGBQUAD[dst_width * dst_height];

		HorizontalFilter(dst_width, dst_height);
	}

	delete m_pRGB;
	m_pRGB = m_pResImg;
	width = dst_width;
	height = dst_height;

	DeleteObject(m_hBMP);
	m_hBMP = 0;
}