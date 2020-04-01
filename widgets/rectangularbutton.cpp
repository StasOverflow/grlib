//*****************************************************************************
//
// rectangularbutton.cpp - Drawing a rectangular button widget.
//
//*****************************************************************************

#include "debug.h"
#include "grlib.h"
#include "widget.h"
#include "rectangularbutton.h"

//*****************************************************************************
//
//! \addtogroup rectangularbutton_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Constructs a rectangular button widget.
//!
//! \param lX is the X coordinate of the upper left corner of the rectangular button.
//! \param lY is the Y coordinate of the upper left corner of the rectangular button.
//! \param lWidth is the width of the rectangular button.
//! \param lHeight is the height of the rectangular button.
//!
//! This function constructs a rectangular button widget.
//!
//! \return None.
//
//*****************************************************************************
RectangularButton::RectangularButton(unsigned char ucID, Widget *pParent,
		                             long lX, long lY, long lWidth, long lHeight) :
	                                 Widget(ucID, pParent, lX, lY, lWidth, lHeight)
{
	//
	// Set initial values for all variables
	//
	this->ulStyle           = 0;
	this->ulFillColor       = 0;
	this->ulPressFillColor  = 0;
	this->ulOutlineColor    = 0;
	this->ulTextColor       = 0;
	this->pFont             = 0;
	this->pcText            = 0;
	this->pucImage          = 0;
	this->pucPressImage     = 0;
	this->usAutoRepeatDelay = 0;
	this->usAutoRepeatRate  = 0;
	this->ulAutoRepeatTimer = 0;
	this->bAutoRepeat       = 0;
}

//*****************************************************************************
//
//! Draws a rectangular button.
//!
//! This function draws a rectangular button on the display.  This is
//! called in response to a \b #WIDGET_MSG_UPDATE message.
//!
//! \return None.
//
//*****************************************************************************
void
RectangularButton::Draw(void)
{
    const unsigned char *pucImage;
    tRectangle sPosition;
    tContext sCtx;
    long lX, lY;
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
    // Initialize the clipping region based on the extents of this rectangular
    // rectangular button.
    //
    GrContextClipRegionSet(&sCtx, &sPosition);

    //
    // See if the rectangular button outline style is selected.
    //
    if(this->ulStyle & RB_STYLE_OUTLINE)
    {
        //
        // Outline the rectangular button with the outline color.
        //
        GrContextForegroundSet(&sCtx, this->ulOutlineColor);
        GrRectDraw(&sCtx, &sPosition);

        sCtx.sClipRegion.sXMin++;
        sCtx.sClipRegion.sYMin++;
        sCtx.sClipRegion.sXMax--;
        sCtx.sClipRegion.sYMax--;
    }

    //
    // See if the rectangular button fill style is selected.
    //
    if(this->ulStyle & RB_STYLE_FILL)
    {
    	if(this->ulStyle & RB_STYLE_FALLING_EDGE)
    	{
    		//
    		// Draw left and top edges
    		//
			if(this->ulStyle & RB_STYLE_PRESSED)
			{
				lColor1 = this->ulPressFillColor;
				lColor2 = this->ulPressFillColor;

	        	ColorValueAdjust(&lColor1, -50);
	        	ColorValueAdjust(&lColor2, -15);
			}
			else
			{
				lColor1 = this->ulFillColor;
				lColor2 = this->ulFillColor;

				ColorValueAdjust(&lColor1, 30);
//				ColorValueAdjust(&lColor2, 30);
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
			if(this->ulStyle & RB_STYLE_PRESSED)
			{
				lColor1 = this->ulPressFillColor;
				lColor2 = this->ulPressFillColor;

				ColorValueAdjust(&lColor1, 30);
//				ColorValueAdjust(&lColor2, 30);
			}
			else
			{
				lColor1 = this->ulFillColor;
				lColor2 = this->ulFillColor;

				ColorValueAdjust(&lColor1, -50);
				ColorValueAdjust(&lColor2, -15);
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
        // Fill the rectangular button with the fill color.
        //
        GrContextForegroundSet(&sCtx, ((this->ulStyle & RB_STYLE_PRESSED) ?
                                       this->ulPressFillColor :
                                       this->ulFillColor));
        GrRectFill(&sCtx, &sPosition);
    }
    else if(this->ulStyle & RB_STYLE_GRADIENT)
    {
    	if( this->ulStyle & RB_STYLE_PRESSED )
    	{
    		GrContextGradientSet(&sCtx, this->ulPressFillColor,
				                        this->ulFillColor);
    	}
    	else
    	{
    		GrContextGradientSet(&sCtx, this->ulFillColor,
				                        this->ulPressFillColor);
    	}

    	GrRectGradientFill(&sCtx, &sPosition);
    }

	//
	// See if the rectangular button image style is selected.
	//
	if(this->ulStyle & RB_STYLE_IMG)
	{
		//
		// Set the foreground and background colors to use for 1 BPP
		// images.
		//
		GrContextForegroundSet(&sCtx, this->ulTextColor);
		GrContextBackgroundSet(&sCtx,
							   ((this->ulStyle & RB_STYLE_PRESSED) ?
								this->ulPressFillColor :
								this->ulFillColor));

		//
		// Get the image to be drawn.
		//
		pucImage = (((this->ulStyle & RB_STYLE_PRESSED) &&
					 this->pucPressImage) ?
					this->pucPressImage : this->pucImage);

		//
		// Compute the coordinates for center image.
		//
		lX = sPosition.sXMin + (((sPosition.sXMax - sPosition.sXMin + 1 - GrImageWidthGet(pucImage)) / 2));
		lY = sPosition.sYMin + (((sPosition.sYMax - sPosition.sYMin + 1 - GrImageHeightGet(pucImage)) / 2));

		//
		// Shift image if button is pressed.
		//
		if( this->ulStyle & RB_STYLE_PRESSED )
		{
			lX++;
			lY++;
		}

		//
		// Draw the image centered in the rectangular button.
		//
		GrImageDraw(&sCtx, pucImage, lX, lY);
	}

	//
	// See if the rectangular button text style is selected.
	//
	if(this->ulStyle & RB_STYLE_TEXT)
	{
		//
		// Set font, foreground and background colors.
		//
		GrContextFontSet(&sCtx, this->pFont);

		if(this->ulStyle & RB_STYLE_DISABLED)
		{
			GrContextForegroundSet(&sCtx, ClrGray);
		}
		else
		{
			GrContextForegroundSet(&sCtx, this->ulTextColor);
		}

		GrContextBackgroundSet(&sCtx,
							   ((this->ulStyle & RB_STYLE_PRESSED) ?
								this->ulPressFillColor :
								this->ulFillColor));

		//
		// Compute the coordinates for center text.
		//
		lX = sPosition.sXMin + (((sPosition.sXMax - sPosition.sXMin + 1 - GrStringWidthGet(&sCtx, this->pcText, -1)) / 2));
		lY = sPosition.sYMin + (((sPosition.sYMax - sPosition.sYMin + 1 - GrStringHeightGet(&sCtx)) / 2));

		//
		// Shift text if button is pressed.
		//
		if( this->ulStyle & RB_STYLE_PRESSED )
		{
			lX++;
			lY++;
		}

		GrStringDraw(&sCtx, this->pcText, -1, lX, lY, this->ulStyle & RB_STYLE_TEXT_OPAQUE);
	}
}

//*****************************************************************************
//
//! Handles pointer events for a rectangular button.
//!
//! \param ulMsg is the pointer event message.
//! \param lX is the X coordinate of the pointer event.
//! \param lY is the Y coordinate of the pointer event.
//!
//! This function processes pointer event messages for a rectangular
//! button.  This is called in response to a \b #WIDGET_MSG_PTR_DOWN,
//! \b #WIDGET_MSG_PTR_MOVE, and \b #WIDGET_MSG_PTR_UP messages.
//!
//! \return Returns 1 if the coordinates are within the extents of the rectangular
//! button and 0 otherwise.
//
//*****************************************************************************
long
RectangularButton::Click(unsigned long ulMsg, long lX, long lY)
{
	tRectangle sPosition;
	long       lRet = 0;

	this->AbsolutePositionGet(&sPosition);

    if(!(this->ulStyle & RB_STYLE_DISABLED))
    {
        switch(ulMsg)
        {
        case WIDGET_MSG_PTR_UP:
        	if(this->ulStyle & RB_STYLE_PRESSED)
        	{
    			this->ulStyle &= ~(RB_STYLE_PRESSED);

            	if(GrRectContainsPoint(&sPosition, lX, lY))
    			{
            		Widget::ParentNotify(this, WIDGET_NOTIFY_BTN_UNPRESSED);
    			}

    			lRet = 1;
        	}

        	break;

        case WIDGET_MSG_PTR_DOWN:
        	if(GrRectContainsPoint(&sPosition, lX, lY))
        	{
                this->ulStyle |= RB_STYLE_PRESSED;

    			this->ulAutoRepeatTimer = GrTimerGet();
    			this->bAutoRepeat = 0;

    			Widget::ParentNotify(this, WIDGET_NOTIFY_BTN_PRESSED);

    			lRet = 1;
        	}

        	break;

        case WIDGET_MSG_PTR_MOVE:
        	if(this->ulStyle & RB_STYLE_PRESSED)
        	{
        		if(this->ulStyle & RB_STYLE_AUTO_REPEAT)
        		{
    				if(!bAutoRepeat)
    				{
    					if((GrTimerGet() - this->ulAutoRepeatTimer) >= this->usAutoRepeatDelay)
    					{
    						bAutoRepeat = 1;

    						this->ulAutoRepeatTimer = GrTimerGet();

    						Widget::ParentNotify(this, WIDGET_NOTIFY_BTN_PRESSED);
    					}
    				}
    				else
    				{
    					if((GrTimerGet() - this->ulAutoRepeatTimer) >= this->usAutoRepeatRate)
    					{
    						this->ulAutoRepeatTimer = GrTimerGet();

    						Widget::ParentNotify(this, WIDGET_NOTIFY_BTN_PRESSED);
    					}
    				}
        		}

        		lRet = 1;
        	}

        	break;
        }
    }

    return lRet;
}

//*****************************************************************************
//
//! Handles messages for a rectangular button widget.
//!
//! \param ulMsg is the message.
//! \param ulParam1 is the first parameter to the message.
//! \param ulParam2 is the second parameter to the message.
//!
//! This function receives messages intended for this rectangular button widget and
//! processes them accordingly.  The processing of the message varies based on
//! the message in question.
//!
//! \return Returns a value appropriate to the supplied message.
//
//*****************************************************************************
long
RectangularButton::MessageProc(unsigned long ulMsg, unsigned long ulParam1,
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
        // One of the pointer requests has been sent.
        //
        case WIDGET_MSG_PTR_DOWN:
        case WIDGET_MSG_PTR_MOVE:
        case WIDGET_MSG_PTR_UP:
        {
            //
            // Handle the pointer request, returning the appropriate value.
            //
            return(this->Click(ulMsg, ulParam1, ulParam2));
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
//! Destroys a rectangular button widget.
//!
//! This function destroys a rectangular button widget.
//!
//! \return None.
//
//*****************************************************************************
RectangularButton::~RectangularButton(void)
{

}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
