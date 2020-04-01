//*****************************************************************************
//
// canvas.cpp - Drawing a canvas widget.
//
//*****************************************************************************

#include <grlib/grlib.h>
#include "widget.h"
#include "canvas.h"

//*****************************************************************************
//
//! \addtogroup canvas_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Constructs a canvas widget.
//!
//! \param lX is the X coordinate of the upper left corner of the canvas.
//! \param lY is the Y coordinate of the upper left corner of the canvas.
//! \param lWidth is the width of the canvas.
//! \param lHeight is the height of the canvas.
//!
//! This function constructs a canvas widget.
//!
//! \return None.
//
//*****************************************************************************
Canvas::Canvas(unsigned char ucID, Widget *pParent, long lX, long lY, long lWidth, long lHeight) :
	    Widget(ucID, pParent, lX, lY, lWidth, lHeight)
{
    //
    // Set initial values for all variables
    //
    this->ulStyle          = 0;
    this->ulFillColor      = 0;
    this->ulGradientColor1 = 0;
    this->ulGradientColor2 = 0;
    this->ulOutlineColor   = 0;
    this->ulTextColor      = 0;
    this->pFont            = 0;
    this->pcText           = 0;
    this->pucImage         = 0;
}

//*****************************************************************************
//
//! Handles messages for a canvas widget.
//!
//! \param ulMsg is the message.
//! \param ulParam1 is the first parameter to the message.
//! \param ulParam2 is the second parameter to the message.
//!
//! This function receives messages intended for this canvas widget and
//! processes them accordingly.  The processing of the message varies based on
//! the message in question.
//!
//! \return Returns a value appropriate to the supplied message.
//
//*****************************************************************************
long
Canvas::MessageProc(unsigned long ulMsg, unsigned long ulParam1,
		            unsigned long ulParam2)
{
    //
    // Determine which message is being sent.
    //
    switch(ulMsg)
    {
        //
        // The widget paint request has been sent.
        //
        case WIDGET_MSG_UPDATE:
        {
        	//
        	// Notify parent
        	//
        	Widget::ParentNotify(this, WIDGET_NOTIFY_UPDATE);

            //
            // Handle the widget paint request.
            //
        	this->Draw();

            //
            // Return one to indicate that the message was successfully
            // processed.
            //
            return(1);
        }

        //
        // An unknown request has been sent.
        //
        default:
        {
            return(0);
        }
    }
}


//*****************************************************************************
//
//! Draws the contents of a canvas.
//!
//! This function draws the contents of a canvas on the display.  This is
//! called in response to a \b #WIDGET_MSG_UPDATE message.
//!
//! \return None.
//
//*****************************************************************************
void
Canvas::Draw(void)
{
	tRectangle sPosition;
    tContext sCtx;
    long lX, lY, lWidth, lHeight;
    long lColor1, lColor2;

    //
    // Remap position of this canvas
    //
    this->AbsolutePositionGet(&sPosition);

    //
    // Initialize a drawing context.
    //
    GrContextInit(&sCtx);

    //
    // Initialize the clipping region based on the extents of this canvas.
    //
    GrContextClipRegionSet(&sCtx, &sPosition);

    //
    // See if the canvas outline style is selected.
    //
    if(this->ulStyle & CANVAS_STYLE_OUTLINE)
    {
        //
        // Outline the canvas with the outline color.
        //
        GrContextForegroundSet(&sCtx, this->ulOutlineColor);
        GrRectDraw(&sCtx, &sPosition);

        //
        // If the canvas outline style is selected then shrink the clipping
        // region by one pixel on each side so that the outline is not
        // overwritten by the text or image.
        //
		sCtx.sClipRegion.sXMin++;
		sCtx.sClipRegion.sYMin++;
		sCtx.sClipRegion.sXMax--;
		sCtx.sClipRegion.sYMax--;
    }

    //
    // See if any of canvas edge styles are selected.
    //
	if(this->ulStyle & (CANVAS_STYLE_FALLING_EDGE |
			            CANVAS_STYLE_RISING_EDGE))
	{
		//
		// Draw left and top edges
		//
		lColor1 = this->ulFillColor;
		lColor2 = this->ulFillColor;

    	if( this->ulStyle & CANVAS_STYLE_FALLING_EDGE )
    	{
//    		ColorValueAdjust(&lColor1, 30);
    		ColorValueAdjust(&lColor2, 30);
    	}
		else if( this->ulStyle & CANVAS_STYLE_RISING_EDGE )
		{
    		ColorValueAdjust(&lColor1, -15);
    		ColorValueAdjust(&lColor2, -50);
		}

		GrContextForegroundSet(&sCtx, lColor1);
		GrLineDrawH(&sCtx, sCtx.sClipRegion.sXMin,
				           sCtx.sClipRegion.sXMax - 1,
				           sCtx.sClipRegion.sYMin);
		GrLineDrawV(&sCtx, sCtx.sClipRegion.sXMin,
				           sCtx.sClipRegion.sYMin,
				           sCtx.sClipRegion.sYMax - 1);

		GrContextForegroundSet(&sCtx, lColor2);
		GrLineDrawH(&sCtx, sCtx.sClipRegion.sXMin + 1,
				           sCtx.sClipRegion.sXMax - 2,
				           sCtx.sClipRegion.sYMin + 1);
		GrLineDrawV(&sCtx, sCtx.sClipRegion.sXMin + 1,
				           sCtx.sClipRegion.sYMin + 1,
				           sCtx.sClipRegion.sYMax - 2);

		//
		// Draw right and bottom edges
		//
		lColor1 = this->ulFillColor;
		lColor2 = this->ulFillColor;

    	if( this->ulStyle & CANVAS_STYLE_FALLING_EDGE )
    	{
    		ColorValueAdjust(&lColor1, -50);
    		ColorValueAdjust(&lColor2, -15);
    	}
		else if( this->ulStyle & CANVAS_STYLE_RISING_EDGE )
		{
    		ColorValueAdjust(&lColor1,  30);
    		ColorValueAdjust(&lColor2,   0);
		}

    	GrContextForegroundSet(&sCtx, lColor1);
    	GrLineDrawH(&sCtx, sCtx.sClipRegion.sXMin,
    			           sCtx.sClipRegion.sXMax,
    			           sCtx.sClipRegion.sYMax);
    	GrLineDrawV(&sCtx, sCtx.sClipRegion.sXMax,
    			           sCtx.sClipRegion.sYMin,
    			           sCtx.sClipRegion.sYMax);

    	GrContextForegroundSet(&sCtx, lColor2);
    	GrLineDrawH(&sCtx, sCtx.sClipRegion.sXMin + 1,
    			           sCtx.sClipRegion.sXMax - 1,
    			           sCtx.sClipRegion.sYMax - 1);
    	GrLineDrawV(&sCtx, sCtx.sClipRegion.sXMax - 1,
    			           sCtx.sClipRegion.sYMin + 1,
    			           sCtx.sClipRegion.sYMax - 1);

    	//
    	// Cut clipping region
    	//
		sCtx.sClipRegion.sXMin += 2;
		sCtx.sClipRegion.sYMin += 2;
		sCtx.sClipRegion.sXMax -= 2;
		sCtx.sClipRegion.sYMax -= 2;
	}

    //
    // See if the canvas fill style is selected.
    //
    if(this->ulStyle & CANVAS_STYLE_FILL)
    {
        //
        // Fill the canvas with the fill color.
        //
        GrContextForegroundSet(&sCtx, this->ulFillColor);
        GrRectFill(&sCtx, &sPosition);
    }

    //
    // See if the canvas text or image style is selected.
    //
    if(this->ulStyle & (CANVAS_STYLE_TEXT | CANVAS_STYLE_IMG | CANVAS_STYLE_GRADIENT))
    {
        //
        // Compute the center of the canvas.
        //
        lX = (sPosition.sXMin +
              ((sPosition.sXMax - sPosition.sXMin + 1) / 2));
        lY = (sPosition.sYMin +
              ((sPosition.sYMax - sPosition.sYMin + 1) / 2));

        if(this->ulStyle & CANVAS_STYLE_GRADIENT)
        {
    		GrContextGradientSet(&sCtx, this->ulGradientColor1,
    				                    this->ulGradientColor2);

        	GrRectGradientFill(&sCtx, &sPosition);
        }
        else if(this->ulStyle & CANVAS_STYLE_IMG)
        {
            //
            // Set the foreground and background colors to use for 1 BPP
            // images.
            //
            GrContextForegroundSet(&sCtx, this->ulTextColor);
            GrContextBackgroundSet(&sCtx, this->ulFillColor);

            //
            // Draw the image centered in the canvas.
            //
            GrImageDraw(&sCtx, this->pucImage,
                        lX - (GrImageWidthGet(this->pucImage) / 2),
                        lY - (GrImageHeightGet(this->pucImage) / 2));
        }

        //
        // See if the canvas text style is selected.
        //
        if(this->ulStyle & CANVAS_STYLE_TEXT)
        {
            //
            // Set the relevant font and colors.
            //
            GrContextFontSet(&sCtx, this->pFont);
            GrContextForegroundSet(&sCtx, this->ulTextColor);
            GrContextBackgroundSet(&sCtx, this->ulFillColor);

            //
            // Determine the drawing position for the string based on the
            // text alignment style.  First consider the horizontal case.  We
            // enter this section with lX set to the center of the widget.
            //

            //
            // How wide is the string?
            //
            lWidth = GrStringWidthGet(&sCtx, this->pcText, -1);

            if(this->ulStyle & CANVAS_STYLE_TEXT_LEFT)
            {
                //
                // The string is to be aligned with the left edge of
                // the widget.  Use the clipping rectangle as reference
                // since this will ensure that the string doesn't
                // encroach on any border that is set.
                //
                lX = sCtx.sClipRegion.sXMin;
            }
            else
            {
                if(this->ulStyle & CANVAS_STYLE_TEXT_RIGHT)
                {
                    //
                    // The string is to be aligned with the right edge of
                    // the widget.  Use the clipping rectangle as reference
                    // since this will ensure that the string doesn't
                    // encroach on any border that is set.
                    //
                    lX = sCtx.sClipRegion.sXMax - lWidth;
                }
                else
                {
                    //
                    // We are centering the string horizontally so adjust
                    // the position accordingly to take into account the
                    // width of the string.
                    //
                    lX -= (lWidth / 2);
                }
            }

            //
            // Now consider the horizontal case.  We enter this section with lY
            // set to the center of the widget.
            //
            // How tall is the string?
            //
            lHeight = GrStringHeightGet(&sCtx);

            if(this->ulStyle & CANVAS_STYLE_TEXT_TOP)
            {
                //
                // The string is to be aligned with the top edge of
                // the widget.  Use the clipping rectangle as reference
                // since this will ensure that the string doesn't
                // encroach on any border that is set.
                //
                lY = sCtx.sClipRegion.sYMin;
            }
            else
            {
                if(this->ulStyle & CANVAS_STYLE_TEXT_BOTTOM)
                {
                    //
                    // The string is to be aligned with the bottom edge of
                    // the widget.  Use the clipping rectangle as reference
                    // since this will ensure that the string doesn't
                    // encroach on any border that is set.
                    //
                    lY = sCtx.sClipRegion.sYMax - lHeight;
                }
                else
                {
                    //
                    // We are centering the string vertically so adjust
                    // the position accordingly to take into account the
                    // height of the string.
                    //
                    lY -= (lHeight / 2);
                }
            }

            //
            // Now draw the string.
            //
            GrStringDraw(&sCtx, this->pcText, -1, lX, lY,
                         this->ulStyle & CANVAS_STYLE_TEXT_OPAQUE);
        }
    }

    //
    // See if this widget have CANVAS_STYLE_PAINT_NOTIFY style.
    //
    if(this->ulStyle & CANVAS_STYLE_PAINT_NOTIFY)
    {
    	//
    	// Send message to parent
    	//
    	Widget::ParentNotify(this, WIDGET_NOTIFY_PAINT);
    }
}

//*****************************************************************************
//
//! Destroys a canvas widget.
//!
//! This function destroys a canvas widget.
//!
//! \return None.
//
//*****************************************************************************
Canvas::~Canvas(void)
{

}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
