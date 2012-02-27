#pragma once
#include "Filters.h"
#include "ImageFile.h"

class CWeightsTable
{
	typedef struct 
	{
		double *Weights;			// Normalized weights of neighboring pixels
		int Left, Right;			// Bounds of source pixels window
	} sContribution;

private:
	// Row (or column) of contribution weights
	sContribution *m_WeightTable;
	// Filter window size (of affecting source pixels)
	DWORD m_WindowSize;
	// Length of line (no. of rows / cols)
	DWORD m_LineLength;

public:
	
	CWeightsTable(CGenericFilter *pFilter, DWORD uDstSize, DWORD uSrcSize);
	~CWeightsTable();

	// Retrieve a filter weight, given source and destination positions
	double getWeight(int dst_pos, int src_pos) {
			return m_WeightTable[dst_pos].Weights[src_pos];
	}

	// Retrieve left boundary of source line buffer
	int getLeftBoundary(int dst_pos) {
			return m_WeightTable[dst_pos].Left;
	}

	// Retrieve right boundary of source line buffer
	int getRightBoundary(int dst_pos) {
			return m_WeightTable[dst_pos].Right;
	}
};


class CResizableImage : public CImageFile
{
	CGenericFilter *m_pFilter;
	RGBQUAD *m_pResImg;
	CWeightsTable *m_pWeights;

public:
	CResizableImage() { m_pFilter = NULL; }
	virtual ~CResizableImage() {}

	void SetFilter(CGenericFilter *pFilter) { m_pFilter = pFilter; }

	// Scale an image to the desired dimensions
	void Resample(unsigned dst_width, unsigned dst_height);

private:
	void ScaleRow(unsigned int dst_width, unsigned int /*dst_height*/, unsigned int row);
	void ScaleCol(unsigned int dst_width, unsigned int dst_height, unsigned int col);

	// Performs horizontal image filtering
	void HorizontalFilter(unsigned int dst_width, unsigned int dst_height);

	// Performs vertical image filtering
	void VerticalFilter(unsigned int dst_width, unsigned int dst_height);
};

