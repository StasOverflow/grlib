//*****************************************************************************
//
// circularbutton.cpp - Drawing a circular button widget.
//
//*****************************************************************************

#include "debug.h"
#include "grlib.h"
#include "widget.h"
#include "circularbutton.h"

//*****************************************************************************
//
//! \addtogroup circularbutton_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Constructs a circular button widget.
//!
//! \param lX is the X coordinate of the upper left corner of the circular button.
//! \param lY is the Y coordinate of the upper left corner of the circular button.
//! \param lR is the radius of the circular button.
//!
//! This function constructs a circular button widget.
//!
//! \return None.
//
//*****************************************************************************
CircularButton::CircularButton(unsigned char ucID, Widget *pParent, long lX, long lY, long lR) :
		        Widget(ucID, pParent, lX-lR, lY-lR, lR*2, lR*2)
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
//! Draws a circular button.
//!
//! This function draws a circular button on the display.  This is called
//! in response to a \b #WIDGET_MSG_UPDATE message.
//!
//! \return None.
//
//*****************************************************************************
void
CircularButton::Draw(void)
{
    const unsigned char *pucImage;
	tRectangle sPosition;
    tContext sCtx;
    long lX, lY, lR;

    //
    // Remap position of this canvas
    //
    this->AbsolutePositionGet(&sPosition);

    //
    // Initialize a drawing context.
    //
    GrContextInit(&sCtx);

    //
    // Initialize the clipping region based on the extents of this circular
    // circular button.
    //
    GrContextClipRegionSet(&sCtx, &sPosition);

    //
    // Get the radius of the circular button, along with the X and Y
    // coordinates for its center.
    //
    lR = (sPosition.sXMax - sPosition.sXMin + 1) / 2;
    lX = sPosition.sXMin + lR;
    lY = sPosition.sYMin + lR;

    //
    // See if the circular button outline style is selected.
    //
    if(this->ulStyle & CB_STYLE_OUTLINE)
    {
        //
        // Outline the circular button with the outline color.
        //
        GrContextForegroundSet(&sCtx, this->ulOutlineColor);
        GrCircleDraw(&sCtx, lX, lY, lR);

        sCtx.sClipRegion.sXMin++;
        sCtx.sClipRegion.sYMin++;
        sCtx.sClipRegion.sXMax--;
        sCtx.sClipRegion.sYMax--;
    }

    if(this->ulStyle & CB_STYLE_FILL)
    {
        //
        // Fill the circular button with the fill color.
        //
        GrContextForegroundSet(&sCtx, ((this->ulStyle & CB_STYLE_PRESSED) ?
                                       this->ulPressFillColor :
                                       this->ulFillColor));
        GrCircleFill(&sCtx, lX, lY, lR);
    }


    //
    // See if the circular button text or image style is selected.
    //
    if(this->ulStyle & (CB_STYLE_TEXT | CB_STYLE_IMG))
    {
        //
        // See if the circular button image style is selected.
        //
        if(this->ulStyle & CB_STYLE_IMG)
        {
            //
            // Set the foreground and background colors to use for 1 BPP
            // images.
            //
            GrContextForegroundSet(&sCtx, this->ulTextColor);
            GrContextBackgroundSet(&sCtx,
                                   ((this->ulStyle & CB_STYLE_PRESSED) ?
                                    this->ulPressFillColor :
                                    this->ulFillColor));

            //
            // Get the image to be drawn.
            //
            pucImage = (((this->ulStyle & CB_STYLE_PRESSED) &&
                         this->pucPressImage) ?
                        this->pucPressImage : this->pucImage);

            //
            // Draw the image centered in the circular button.
            //
            GrImageDraw(&sCtx, pucImage, lX - (GrImageWidthGet(pucImage) / 2),
                        lY - (GrImageHeightGet(pucImage) / 2));
        }

        //
        // See if the circular button text style is selected.
        //
        if(this->ulStyle & CB_STYLE_TEXT)
        {
            //
            // Draw the text centered in the middle of the circular button.
            //
            GrContextFontSet(&sCtx, this->pFont);

            if(this->ulStyle & CB_STYLE_DISABLED)
            {
            	GrContextForegroundSet(&sCtx, ClrGray);
            }
            else
            {
            	GrContextForegroundSet(&sCtx, this->ulTextColor);
            }

            GrContextBackgroundSet(&sCtx,
                                   ((this->ulStyle & CB_STYLE_PRESSED) ?
                                    this->ulPressFillColor :
                                    this->ulFillColor));
//            GrStringDrawCentered(&sCtx, this->pcText, -1, lX, lY,
//                                 this->ulStyle & CB_STYLE_TEXT_OPAQUE);

            GrStringDrawCentered(&sCtx, this->pcText, -1, &sPosition,
                                 this->ulStyle & CB_STYLE_TEXT_OPAQUE);
        }
    }
}

//*****************************************************************************
//
//! Handles pointer events for a circular button.
//!
//! \param ulMsg is the pointer event message.
//! \param lX is the X coordinate of the pointer event.
//! \param lY is the Y coordinate of the pointer event.
//!
//! This function processes pointer event messages for a circular button.
//! This is called in response to a \b #WIDGET_MSG_PTR_DOWN,
//! \b #WIDGET_MSG_PTR_MOVE, and \b #WIDGET_MSG_PTR_UP messages.
//!
//! \return Returns 1 if the coordinates are within the extents of the circular
//! button and 0 otherwise.
//
//*****************************************************************************
long
CircularButton::Click(unsigned long ulMsg, long lX, long lY)
{
	tRectangle sPosition;
    long lXc, lYc, lR;
	long lRet = 0;

	this->AbsolutePositionGet(&sPosition);

	lR = (sPosition.sXMax - sPosition.sXMin + 1) / 2;
	lXc = sPosition.sXMin + lR;
	lYc = sPosition.sYMin + lR;

    if(!(this->ulStyle & CB_STYLE_DISABLED))
    {
        switch(ulMsg)
        {
        case WIDGET_MSG_PTR_UP:
        	if(this->ulStyle & CB_STYLE_PRESSED)
        	{
    			this->ulStyle &= ~(CB_STYLE_PRESSED);

            	if(((((lX - lXc) * (lX - lXc)) + ((lY - lYc) * (lY - lYc))) <= (lR * lR)))
    			{
            		Widget::ParentNotify(this, WIDGET_NOTIFY_BTN_UNPRESSED);
    			}

    			lRet = 1;
        	}

        	break;

        case WIDGET_MSG_PTR_DOWN:
        	if(((((lX - lXc) * (lX - lXc)) + ((lY - lYc) * (lY - lYc))) <= (lR * lR)))
        	{
                this->ulStyle |= CB_STYLE_PRESSED;

    			this->ulAutoRepeatTimer = GrTimerGet();
    			this->bAutoRepeat = 0;

    			Widget::ParentNotify(this, WIDGET_NOTIFY_BTN_PRESSED);

    			lRet = 1;
        	}

        	break;

        case WIDGET_MSG_PTR_MOVE:
        	if(this->ulStyle & CB_STYLE_PRESSED)
        	{
        		if(this->ulStyle & CB_STYLE_AUTO_REPEAT)
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
//! Handles messages for a circular button widget.
//!
//! \param ulMsg is the message.
//! \param ulParam1 is the first parameter to the message.
//! \param ulParam2 is the second parameter to the message.
//!
//! This function receives messages intended for this circular button widget and
//! processes them accordingly.  The processing of the message varies based on
//! the message in question.
//!
//! \return Returns a value appropriate to the supplied message.
//
//*****************************************************************************
long
CircularButton::MessageProc(unsigned long ulMsg, unsigned long ulParam1,
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
//! Destroys a circular button widget.
//!
//! This function destroys a circular button widget.
//!
//! \return None.
//
//*****************************************************************************
CircularButton::~CircularButton(void)
{

}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
