/*! 
 *  \brief     ResizableImage class.
 *  \details   This class is responsible for resizing bitmap images.
 *  \author    Mihai Popescu
 *  \date      01/03/2009
 *  \copyright GNU Public License.
 */
#ifndef RESIZABLE_IMAGE_H
#define RESIZABLE_IMAGE_H

#include "ImageFile.h"
#include "WeightsTable.h"


class CResizableImage : public CImageFile
{
public:
	CResizableImage() { myFilter = NULL; }
	virtual ~CResizableImage() { if(myFilter) delete myFilter; }

	void SetFilter(CGenericFilter *pFilter) { myFilter = pFilter; }

	//! Scale an image to the desired dimensions
	void Resample(unsigned dst_width, unsigned dst_height);

private:
	void ScaleRow(unsigned int dst_width, unsigned int /*dst_height*/, unsigned int row);
	void ScaleCol(unsigned int dst_width, unsigned int dst_height, unsigned int col);

	//! Performs horizontal image filtering
	void HorizontalFilter(unsigned int dst_width, unsigned int dst_height);

	//! Performs vertical image filtering
	void VerticalFilter(unsigned int dst_width, unsigned int dst_height);

private:
	CGenericFilter *myFilter;
	RGBQUAD *myResizedImage;
	CWeightsTable *myWeightsTable;
};


#endif // RESIZABLE_IMAGE_H