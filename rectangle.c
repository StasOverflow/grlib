//*****************************************************************************
//
// rectangle.c - Routines for drawing and filling rectangles.
//
//*****************************************************************************

#include "debug.h"
#include "grlib.h"

//*****************************************************************************
//
//! \addtogroup primitives_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Make sure min and max are defined.
//
//*****************************************************************************
#ifndef min
#define min(a, b)               (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a, b)               (((a) < (b)) ? (b) : (a))
#endif

//*****************************************************************************
//
//! Draws a rectangle.
//!
//! \param pContext is a pointer to the drawing context to use.
//! \param pRect is a pointer to the structure containing the extents of the
//! rectangle.
//!
//! This function draws a rectangle.  The rectangle will extend from \e lXMin
//! to \e lXMax and \e lYMin to \e lYMax, inclusive.
//!
//! \return None.
//
//*****************************************************************************
void
GrRectDraw(const tContext *pContext, const tRectangle *pRect)
{
    //
    // Check the arguments.
    //
    ASSERT(pContext);
    ASSERT(pRect);

    //
    // Draw a line across the top of the rectangle.
    //
    GrLineDrawH(pContext, pRect->sXMin, pRect->sXMax, pRect->sYMin);

    //
    // Return if the rectangle is one pixel tall.
    //
    if(pRect->sYMin == pRect->sYMax)
    {
        return;
    }

    //
    // Draw a line down the right side of the rectangle.
    //
    GrLineDrawV(pContext, pRect->sXMax, pRect->sYMin + 1, pRect->sYMax);

    //
    // Return if the rectangle is one pixel wide.
    //
    if(pRect->sXMin == pRect->sXMax)
    {
        return;
    }

    //
    // Draw a line across the bottom of the rectangle.
    //
    GrLineDrawH(pContext, pRect->sXMax - 1, pRect->sXMin, pRect->sYMax);

    //
    // Return if the rectangle is two pixels tall.
    //
    if((pRect->sYMin + 1) == pRect->sYMax)
    {
        return;
    }

    //
    // Draw a line up the left side of the rectangle.
    //
    GrLineDrawV(pContext, pRect->sXMin, pRect->sYMax - 1, pRect->sYMin + 1);
}

//*****************************************************************************
//
//! Draws a filled rectangle.
//!
//! \param pContext is a pointer to the drawing context to use.
//! \param pRect is a pointer to the structure containing the extents of the
//! rectangle.
//!
//! This function draws a filled rectangle.  The rectangle will extend from
//! \e lXMin to \e lXMax and \e lYMin to \e lYMax, inclusive.  The clipping of
//! the rectangle to the clipping rectangle is performed within this routine;
//! the display driver's rectangle fill routine is used to perform the actual
//! rectangle fill.
//!
//! \return None.
//
//*****************************************************************************
void
GrRectFill(const tContext *pContext, const tRectangle *pRect)
{
    tRectangle sTemp;

    //
    // Check the arguments.
    //
    ASSERT(pContext);
    ASSERT(pRect);

    //
    // Swap the X coordinates if sXMin is greater than sXMax.
    //
    if(pRect->sXMin > pRect->sXMax)
    {
        sTemp.sXMin = pRect->sXMax;
        sTemp.sXMax = pRect->sXMin;
    }
    else
    {
        sTemp.sXMin = pRect->sXMin;
        sTemp.sXMax = pRect->sXMax;
    }

    //
    // Swap the Y coordinates if sYMin is greater than sYMax.
    //
    if(pRect->sYMin > pRect->sYMax)
    {
        sTemp.sYMin = pRect->sYMax;
        sTemp.sYMax = pRect->sYMin;
    }
    else
    {
        sTemp.sYMin = pRect->sYMin;
        sTemp.sYMax = pRect->sYMax;
    }

    //
    // Now that the coordinates are ordered, return without drawing anything if
    // the entire rectangle is out of the clipping region.
    //
    if((sTemp.sXMin > pContext->sClipRegion.sXMax) ||
       (sTemp.sXMax < pContext->sClipRegion.sXMin) ||
       (sTemp.sYMin > pContext->sClipRegion.sYMax) ||
       (sTemp.sYMax < pContext->sClipRegion.sYMin))
    {
        return;
    }

    //
    // Clip the X coordinates to the edges of the clipping region if necessary.
    //
    if(sTemp.sXMin < pContext->sClipRegion.sXMin)
    {
        sTemp.sXMin = pContext->sClipRegion.sXMin;
    }
    if(sTemp.sXMax > pContext->sClipRegion.sXMax)
    {
        sTemp.sXMax = pContext->sClipRegion.sXMax;
    }

    //
    // Clip the Y coordinates to the edges of the clipping region if necessary.
    //
    if(sTemp.sYMin < pContext->sClipRegion.sYMin)
    {
        sTemp.sYMin = pContext->sClipRegion.sYMin;
    }
    if(sTemp.sYMax > pContext->sClipRegion.sYMax)
    {
        sTemp.sYMax = pContext->sClipRegion.sYMax;
    }

    //
    // Call the low level rectangle fill routine.
    //
    DisplayRectFill(&sTemp, pContext->ulForeground);
}

//*****************************************************************************
//
//! Determines if two rectangles overlap.
//!
//! \param psRect1 is a pointer to the first rectangle.
//! \param psRect2 is a pointer to the second rectangle.
//!
//! This function determines whether two rectangles overlap.  It assumes that
//! rectangles \e psRect1 and \e psRect2 are valid with \e sXMin < \e sXMax and
//! \e sYMin < \e sYMax.
//!
//! \return Returns 1 if there is an overlap or 0 if not.
//
//*****************************************************************************
long
GrRectOverlapCheck(tRectangle *psRect1, tRectangle *psRect2)
{
    if((psRect1->sXMax < psRect2->sXMin) ||
       (psRect2->sXMax < psRect1->sXMin) ||
       (psRect1->sYMax < psRect2->sYMin) ||
       (psRect2->sYMax < psRect1->sYMin))
    {
        return(0);
    }
    else
    {
        return(1);
    }
}

//*****************************************************************************
//
//! Determines the intersection of two rectangles.
//!
//! \param psRect1 is a pointer to the first rectangle.
//! \param psRect2 is a pointer to the second rectangle.
//! \param psIntersect is a pointer to a rectangle which will be written with
//! the intersection of \e psRect1 and \e psRect2.
//!
//! This function determines if two rectangles overlap and, if they do,
//! calculates the rectangle representing their intersection.  If the rectangles
//! do not overlap, 0 is returned and \e psIntersect is not written.
//!
//! \return Returns 1 if there is an overlap or 0 if not.
//
//*****************************************************************************
long
GrRectIntersectGet(tRectangle *psRect1, tRectangle *psRect2,
                   tRectangle *psIntersect)
{
    //
    // Make sure we were passed valid rectangles.
    //
    if((psRect1->sXMax <= psRect1->sXMin) ||
       (psRect1->sYMax <= psRect1->sYMin) ||
       (psRect2->sXMax <= psRect2->sXMin) ||
       (psRect2->sYMax <= psRect2->sYMin))
    {
        return(0);
    }

    //
    // Make sure that there is an intersection between the two rectangles.
    //
    if(!GrRectOverlapCheck(psRect1, psRect2))
    {
        return(0);
    }

    //
    // The rectangles do intersect so determine the rectangle of the
    // intersection.
    //
    psIntersect->sXMin = max(psRect1->sXMin, psRect2->sXMin);
    psIntersect->sXMax = min(psRect1->sXMax, psRect2->sXMax);
    psIntersect->sYMin = max(psRect1->sYMin, psRect2->sYMin);
    psIntersect->sYMax = min(psRect1->sYMax, psRect2->sYMax);

    return(1);
}


//*****************************************************************************
//
//! Draws a rectangle filled by linear gradient.
//!
//! \param pContext is a pointer to the drawing context to use.
//! \param pRect is a pointer to the structure containing the extents of the
//! rectangle.
//! \param ulColor1 is a first color of gradient filling.
//! \param ulColor2 is a second color of gradient filling.
//!
//! This function draws a rectangle filled by gradient.
//!
//! \return None.
//
//*****************************************************************************
void
GrRectGradientFill( tContext *pContext, const tRectangle *pRect )
{
	tRectangle    sTemp;
	unsigned long ulCurrColor;
	         long lColorRedDiff;
	         long lColorGreenDiff;
	         long lColorBlueDiff;
	         long lColorRed;
	         long lColorGreen;
	         long lColorBlue;

	 //
	 // Check the arguments.
	 //
	 ASSERT(pContext);
	 ASSERT(pRect);

	 //
	 // Swap the X coordinates if sXMin is greater than sXMax.
	 //
	 if(pRect->sXMin > pRect->sXMax)
	 {
		 sTemp.sXMin = pRect->sXMax;
		 sTemp.sXMax = pRect->sXMin;
	 }
	 else
	 {
		 sTemp.sXMin = pRect->sXMin;
		 sTemp.sXMax = pRect->sXMax;
	 }

	 //
	 // Swap the Y coordinates if sYMin is greater than sYMax.
	 //
	 if(pRect->sYMin > pRect->sYMax)
	 {
		 sTemp.sYMin = pRect->sYMax;
		 sTemp.sYMax = pRect->sYMin;
	 }
	 else
	 {
		 sTemp.sYMin = pRect->sYMin;
		 sTemp.sYMax = pRect->sYMax;
	 }

	 //
	 // Now that the coordinates are ordered, return without drawing anything if
	 // the entire rectangle is out of the clipping region.
	 //
	 if((sTemp.sXMin > pContext->sClipRegion.sXMax) ||
		(sTemp.sXMax < pContext->sClipRegion.sXMin) ||
		(sTemp.sYMin > pContext->sClipRegion.sYMax) ||
		(sTemp.sYMax < pContext->sClipRegion.sYMin))
	 {
		 return;
	 }


	lColorRed   = ((pContext->ulGradient1 & 0x00FF0000) >> 16) * 255;
	lColorGreen = ((pContext->ulGradient1 & 0x0000FF00) >> 8)  * 255;
	lColorBlue  = ((pContext->ulGradient1 & 0x000000FF) >> 0)  * 255;

	lColorRedDiff   = (((pContext->ulGradient2 & 0x00FF0000) >> 16) - ((pContext->ulGradient1 & 0x00FF0000) >> 16)) * 255;
	lColorGreenDiff = (((pContext->ulGradient2 & 0x0000FF00) >>  8) - ((pContext->ulGradient1 & 0x0000FF00) >>  8)) * 255;
	lColorBlueDiff  = (((pContext->ulGradient2 & 0x000000FF) >>  0) - ((pContext->ulGradient1 & 0x000000FF) >>  0)) * 255;

	lColorRedDiff   /= sTemp.sYMax - sTemp.sYMin + 1;
	lColorGreenDiff /= sTemp.sYMax - sTemp.sYMin + 1;
	lColorBlueDiff  /= sTemp.sYMax - sTemp.sYMin + 1;

	for( short sRow = sTemp.sYMin; sRow <= sTemp.sYMax; sRow++ )
	{
		lColorRed   += lColorRedDiff;
		lColorGreen += lColorGreenDiff;
		lColorBlue  += lColorBlueDiff;

		ulCurrColor  = ((lColorRed   / 255) & 0xFF) << 16;
		ulCurrColor |= ((lColorGreen / 255) & 0xFF) << 8;
		ulCurrColor |= ((lColorBlue  / 255) & 0xFF) << 0;

		GrContextForegroundSet(pContext, ulCurrColor);
		GrLineDrawH(pContext, sTemp.sXMin, sTemp.sXMax, sRow);
	}
}


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
