#include "WeightsTable.h"


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

