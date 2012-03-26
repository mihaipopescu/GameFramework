/*! 
 *  \brief     CWeightsTable class.
 *  \details   This class is used at resize.
 *  \author    Mihai Popescu
 *  \date      01/03/2009
 *  \copyright GNU Public License.
 */
#ifndef WEIGHTS_TABLE_H
#define WEIGHTS_TABLE_H

#include "main.h"
#include "Filters.h"

class CWeightsTable
{
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

	typedef struct
	{
		double *Weights;			//!< Normalized weights of neighboring pixels
		int Left, Right;			//!< Bounds of source pixels window
	} sContribution;

private:
	sContribution *m_WeightTable; //!< Row (or column) of contribution weights
	DWORD m_WindowSize; //!< Filter window size (of affecting source pixels)
	DWORD m_LineLength; //!< Length of line (no. of rows / cols)
};

#endif // WEIGHTS_TABLE_H