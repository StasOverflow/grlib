//*****************************************************************************
//
// widget.cpp - Generic widget tree handling code.
//
//*****************************************************************************

#include "mqueue.h"
#include "widget.h"

//*****************************************************************************
//
//! \addtogroup widget_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// This structure describes the message queue item used to hold widget
// messages.
//
//*****************************************************************************
typedef struct
{
    //
    // The widget (or widget tree) to which the message should be sent.
    //
    Widget *pWidget;

    //
    // The message to be sent.
    //
    unsigned long ulMessage;

    //
    // The first parameter to the message.
    //
    unsigned long ulParam1;

    //
    // The second parameter to the message.
    //
    unsigned long ulParam2;

    //
    // The flags that describe how this message should be processed; this is
    // defined by the MSG_FLAG_xxx flags.
    //
    unsigned long ulFlags;

} tMessageQueueItem;

//*****************************************************************************
//
//! The pointer to the message queue object.
//
//*****************************************************************************
MQueue *Widget::pMQueue;

//*****************************************************************************
//
// Pointer to the root of the widget tree. This is the widget used when no parent is
// specified when adding a widget, or when no widget is specified when sending
// a message.  The parent and sibling of this widget are always zero.  This
// should not be directly referenced by applications; WIDGET_ROOT should be
// used instead.
//
//*****************************************************************************
Widget *g_pcRootWidget = 0;

//*****************************************************************************
//
//! Sends a pointer message.
//!
//! \param ulMessage is the pointer message to be sent.
//! \param lX is the X coordinate associated with the message.
//! \param lY is the Y coordinate associated with the message.
//!
//! This function sends a pointer message to the root widget.  A pointer driver
//! (such as a touch screen driver) can use this function to deliver pointer
//! activity to the widget tree without having to have direct knowledge of the
//! structure of the widget framework.
//!
//! \return Returns 1 if the message was added to the queue, and 0 if it could
//! not be added since the queue is full.
//
//*****************************************************************************
long
WidgetPointerMessage(unsigned long ulMessage, long lX, long lY)
{
    //
    // Add the message to the widget message queue.
    //
    return(Widget::MessagePost(g_pcRootWidget,
    		                   ulMessage, lX, lY,
    		                   MSG_FLAG_POST_ORDER |
    		                   MSG_FLAG_STOP_ON_SUCCESS));
}

//*****************************************************************************
//
//! Creates a widget object.
//!
//! This function creates a widget object.
//!
//! \return None.
//
//*****************************************************************************
Widget::Widget(unsigned char ucID, Widget *pParent, long lX, long lY, long lWidth, long lHeight)
{
	this->ucID     = ucID;

    this->sPosition.sXMin = lX;
    this->sPosition.sYMin = lY;
    this->sPosition.sXMax = lX + lWidth - 1;
    this->sPosition.sYMax = lY + lHeight - 1;

    if(pParent)
    {
    	pParent->AppendChild(this);
    }

    if(!Widget::pMQueue)
    {
    	Widget::pMQueue = new MQueue(sizeof(tMessageQueueItem), 16);
    }
}

//*****************************************************************************
//
//! Requests a redraw of the widget tree.
//!
//! This function sends a \b #WIDGET_MSG_UPDATE message to the given widgets,
//! and all of the widget beneath it, so that they will draw or redraw
//! themselves on the display.
//!
//! \return None.
//
//*****************************************************************************
void
Widget::Update(void)
{
	//
	// Send WIDGET_MSG_UPDATE message to widget tree
	//
	Widget::MessageSend(this, WIDGET_MSG_UPDATE, 0, 0, 0);

//	//
//	// Flush this widget to the display
//	//
//	this->Flush();
}

//*****************************************************************************
//
//! Moves this widget on display.
//!
//! This function moves this widget on display.
//!
//! \param lX is a relative X coordinate for new widget position.
//! \param lY is a relative Y coordinate for new widget position.
//!
//! \return None.
//
//*****************************************************************************
void
Widget::Move(long lX, long lY)
{
	this->sPosition.sXMin += lX;
	this->sPosition.sYMin += lY;
	this->sPosition.sXMax += lX;
	this->sPosition.sYMax += lY;
}

//*****************************************************************************
//
//! Flushes a widget to the display.
//!
//! This function flushes a widget rectangle to the display.
//!
//! \return None.
//
//*****************************************************************************
void
Widget::Flush(void)
{
	tRectangle sPosition;

    //
    // Remap position of this canvas
    //
	this->AbsolutePositionGet(&sPosition);

	//
	// Flush rectangle area of the widget to display.
	//
	DisplayFlush(&sPosition);
}

//*****************************************************************************
//
//! Gets an absolute coordinates of this widget.
//!
//! This function Gets an absolute coordinates of this widget.
//!
//! \param psPosition is a pointer to the rectangle that contains an absolute
//! coordinates of this widget.
//!
//! \return None.
//
//*****************************************************************************
void
Widget::AbsolutePositionGet(tRectangle *psPosition)
{
	Widget *pWidget;
	Tree   *pTemp;

	//
	// Set relative position of this widget first
	//
	psPosition->sXMin = this->sPosition.sXMin;
	psPosition->sYMin = this->sPosition.sYMin;
	psPosition->sXMax = this->sPosition.sXMax;
	psPosition->sYMax = this->sPosition.sYMax;

	//
	// Loop through all parents of this widget
	//
	for(pTemp = this->ParentGet(); pTemp;
	    pTemp = pTemp->ParentGet())
	{
		pWidget = (Widget *)pTemp;

		//
		// Accumulate all parents coordinates for get
		// an absolute position of this widget.
		//
		psPosition->sXMin += pWidget->sPosition.sXMin;
		psPosition->sYMin += pWidget->sPosition.sYMin;
		psPosition->sXMax += pWidget->sPosition.sXMin;
		psPosition->sYMax += pWidget->sPosition.sYMin;
	}
}

//*****************************************************************************
//
//! Destroys this widget and all his children.
//!
//! This function destroys this widget and all his children.
//!
//! \return None.
//
//*****************************************************************************
void
Widget::Destroy(void)
{
    Tree *pTemp;
    Tree *pPrev;
    Tree *pParent;

    //
    // Detach the widget first
    //
    this->Cut();

    pTemp = this;

    //
    // Loop through the tree under the widget until every widget is searched.
    //
    do
    {
        //
        // See if this widget has a child.
        //
        if(pTemp->ChildGet())
        {
            //
            // Go to this widget's child first.
            //
            pTemp = pTemp->ChildGet();

        	//
        	// Go to the last siblig widget at this level
        	//
        	while(pTemp->NextGet())
        	{
        		pTemp = pTemp->NextGet();
        	}
        }

        //
        // This widget does not have a child, so either a sibling or a parent
        // must be checked.  When moving back to the parent, another move must
        // be performed as well to avoid getting stuck in a loop (since the
        // parent's children have already been searched.
        //
        else
        {
            //
            // Loop until returning to the parent of the starting widget.  This
            // loop will be explicitly broken out of if an intervening widget
            // is encountered that has not been searched.
            //
        	while(pTemp != this)
            {
        		//
        		// Save widget's parent before destroing.
        		//
        		pParent = pTemp->ParentGet();

				//
				// Notify parent before destroy widget.
				//
				Widget::ParentNotify((Widget *)pTemp, WIDGET_NOTIFY_DESTROY);

				//
				// Destroys this widget.
				//
				delete pTemp;

			    //
			    // See if this widget is not the first child of its parent.
			    //
                if(pParent->ChildGet() != pTemp)
                {
					//
					// Find the sibling directly before this widget (previous).
					//
					for(pPrev = pParent->ChildGet(); pPrev->NextGet() != pTemp;
						pPrev = pPrev->NextGet())
					{
					}


                    //
                    // Visit to the previous sibling of this widget.
                    //
                    pTemp = pPrev;

                    //
                    // Since this widget has not been searched yet, break out
                    // of the controlling loop.
                    //
                    break;
                }
                else
                {
                    //
                    // This widget has no siblings, so go to its parent.  Since
                    // the parent has already been searched, the same sibling
                    // vs. parent decision must be made on this widget as well.
                    //
                    pTemp = pParent;
                }
            }
        }
    }
    while(pTemp != this);

	//
	// Notify parent before destroy widget.
	//
	Widget::ParentNotify(this, WIDGET_NOTIFY_DESTROY);

    //
    // Destroys this widget.
    //
	delete this;
}

//*****************************************************************************
//
//! Adds message to the widget message queue.
//!
//! \param ulMessage is a message to be added to the widget queue.
//! \param ulParam1 is a first paramener of the message.
//! \param ulParam2 is a second paramener of the message.
//! \param ulFlags is a flags that describe how this message should be
//! processed.
//!
//! This function places a widget message into the message queue for later
//! processing.
//!
//! It is safe for code which interrupts WidgetMessageQueueProcess() (or called
//! by it) to call this function to send a message.  It is not safe for code
//! which interrupts this function to call this function as well; it is up to
//! the caller to guarantee that the later sequence never occurs.
//!
//! \return Returns 1 if the message was added to the queue, and 0 if it could
//! not be added since either the queue is full or another context is currently
//! adding a message to the queue.
//
//*****************************************************************************
long
Widget::MessagePost(Widget *pWidget, unsigned long ulMessage,
		            unsigned long ulParam1,
		            unsigned long ulParam2,
		            unsigned long ulFlags)
{
    tMessageQueueItem sMessageQueueItem;

    //
    // If the queue is not empty, and this is a pointer move message, see if
    // the previous message was also a move and, if so, replace the
    // coordinates.  Without this, the message queue can very quickly overflow
    // if the application is busy doing something while the user keeps pressing
    // the display.
    //
    if(ulMessage == WIDGET_MSG_PTR_MOVE)
    {
    	if(Widget::pMQueue->Show(&sMessageQueueItem))
    	{
    		if(sMessageQueueItem.ulMessage == WIDGET_MSG_PTR_MOVE)
    		{
    		    sMessageQueueItem.pWidget   = pWidget;
    		    sMessageQueueItem.ulMessage = ulMessage;
    		    sMessageQueueItem.ulParam1  = ulParam1;
    		    sMessageQueueItem.ulParam2  = ulParam2;
    		    sMessageQueueItem.ulFlags   = ulFlags;

    		    return Widget::pMQueue->OverWrite(&sMessageQueueItem);
    		}
    	}
    }

	sMessageQueueItem.pWidget   = pWidget;
	sMessageQueueItem.ulMessage = ulMessage;
	sMessageQueueItem.ulParam1  = ulParam1;
	sMessageQueueItem.ulParam2  = ulParam2;
	sMessageQueueItem.ulFlags   = ulFlags;

	return Widget::pMQueue->EnQueue(&sMessageQueueItem);
}


//*****************************************************************************
//
//! Sends a message to a widget tree.
//!
//! \param ulMessage is the message to send.
//! \param ulParam1 is the first parameter to the message.
//! \param ulParam2 is the second parameter to the message.
//! \param ulFlags is a flags that describe how this message should be
//! processed.
//!
//! This function sends a message directly to the widget
//!
//! \return Returns 1 if the widget to successfully process the message.
//
//*****************************************************************************
long
Widget::MessageSend(Widget *pWidget, unsigned long ulMessage,
		            unsigned long ulParam1, unsigned long ulParam2,
		            unsigned long ulFlags)
{
    //
    // See if this message should be sent via a post-order or pre-order
    // search.
    //
    if(ulFlags & MSG_FLAG_POST_ORDER)
    {
        //
        // Send this message with a post-order search of the widget tree.
        //
    	return Widget::MessageSendPostOrder(pWidget,
    			                            ulMessage,
    			                            ulParam1,
    			                            ulParam2,
                                            (ulFlags & MSG_FLAG_STOP_ON_SUCCESS) ?
                                             1 : 0,
    			                            (ulFlags & MSG_FLAG_STOP_ON_FIRST) ?
                                             1 : 0);
    }
    else
    {
        //
        // Send this message with a pre-order search of the widget tree.
        //
    	return Widget::MessageSendPreOrder(pWidget,
    			                           ulMessage,
    			                           ulParam1,
    			                           ulParam2,
                                           (ulFlags & MSG_FLAG_STOP_ON_SUCCESS) ?
                                            1 : 0,
                                           (ulFlags & MSG_FLAG_STOP_ON_FIRST) ?
                                            1 : 0);
    }
}

//*****************************************************************************
//
//! Sends notification message to parent.
//!
//! This function sends notification message to parent.
//!
//! \param ulMessage is the message to send.
//!
//! \return Returns 1 if success.
//
//*****************************************************************************
long Widget::ParentNotify(Widget *pWidget, unsigned long ulMessage)
{
	Widget *pParent;

	pParent = (Widget *)pWidget->ParentGet();

	if(pParent)
	{
		return Widget::MessageSend(pParent,
								   ulMessage,
								   pWidget->ucID,
								   (unsigned long)pWidget,
								   MSG_FLAG_STOP_ON_FIRST);
	}
	else
	{
		return 0;
	}
}

//*****************************************************************************
//
//! Sends a message to a widget tree via a pre-order, depth-first search.
//!
//! \param ulMessage is the message to send.
//! \param ulParam1 is the first parameter to the message.
//! \param ulParam2 is the second parameter to the message.
//! \param bStopOnSuccess is true if the search should be stopped when the
//! first widget is found that returns success in response to the message.
//!
//! This function performs a pre-order, depth-first search of the widget tree,
//! sending a message to each widget encountered.  In a depth-first search, the
//! children of a widget are searched before its siblings (preferring to go
//! deeper into the tree, hence the name depth-first).  A pre-order search
//! means that the message is sent to a widget before any of its children are
//! searched.
//!
//! An example use of the pre-order search is for paint messages; the larger
//! enclosing widgets should be drawn on the screen before the smaller widgets
//! that reside within the parent widget (otherwise, the children would be
//! overwritten by the parent).
//!
//! \return Returns 0 if \e bStopOnSuccess is false or no widget returned
//! success in response to the message, or the value returned by the first
//! widget to successfully process the message.
//
//*****************************************************************************
long
Widget::MessageSendPreOrder(Widget *pWidget, unsigned long ulMessage,
                            unsigned long ulParam1, unsigned long ulParam2,
                            char bStopOnSuccess, char bStopAfterFirst)
{
    Tree *pTree = 0;
    long  lRet;

	//
	// Loop through the tree under the widget until every widget is searched.
	//
	do
	{
		pTree = pWidget->PreOrderSearch(pTree);

		if(pTree)
		{
			//
			// Send the message to this widget and return if it succeeded and the
			// search should stop on success.
			//
			lRet = ((Widget *)pTree)->MessageProc(ulMessage, ulParam1, ulParam2);

			if(((lRet != 0) && bStopOnSuccess) || bStopAfterFirst)
			{
				return(lRet);
			}
		}
	}
	while(pTree);

    //
    // No widget returned success for the message, or bStopOnSuccess was zero,
    // so return zero.
    //
    return(0);
}


//*****************************************************************************
//
//! Sends a message to a widget tree via a post-order, depth-first search.
//!
//! \param ulMessage is the message to send.
//! \param ulParam1 is the first parameter to the message.
//! \param ulParam2 is the second parameter to the message.
//! \param bStopOnSuccess is true if the search should be stopped when the
//! first widget is found that returns success in response to the message.
//!
//! This function performs a post-order, depth-first search of the widget tree,
//! sending a message to each widget encountered.  In a depth-first search, the
//! children of a widget are searched before its sibling (preferring to go
//! deeper into the tree, hence the name depth-first).  A post-order search
//! means that the message is sent to a widget after all of its children are
//! searched.
//!
//! An example use of the post-order search is for pointer-related messages;
//! those messages should be delivered to the lowest widget in the tree before
//! its parents (in other words, the widget deepest in the tree that has a hit
//! should get the message, not the higher up widgets that also include the hit
//! location).
//!
//! Special handling is performed for pointer-related messages.  The widget
//! that accepts \b #WIDGET_MSG_PTR_DOWN is remembered and subsequent
//! \b #WIDGET_MSG_PTR_MOVE and \b #WIDGET_MSG_PTR_UP messages are sent
//! directly to that widget.
//!
//! \return Returns 0 if \e bStopOnSuccess is false or no widget returned
//! success in response to the message, or the value returned by the first
//! widget to successfully process the message.
//
//*****************************************************************************
long
Widget::MessageSendPostOrder(Widget *pWidget, unsigned long ulMessage,
                             unsigned long ulParam1, unsigned long ulParam2,
                             char bStopOnSuccess, char bStopAfterFirst)
{
    Widget *pTemp;
    Tree   *pTree = 0;
    long    lRet;

	do
	{
		pTree = pWidget->PostOrderSearch(pTree);

		if(pTree)
		{
			pTemp = (Widget *)pTree;

			//
			// Send the message to this widget.
			//
			lRet = pTemp->MessageProc(ulMessage, ulParam1, ulParam2);

			//
			// If the widget returned success and the search should stop on
			// success then return immediately.
			//
			if(((lRet != 0) && bStopOnSuccess) || bStopAfterFirst)
			{
				return(lRet);
			}
		}
	}
	while(pTree);

    //
    // No widget returned success for the message, or bStopOnSuccess was zero,
    // so return zero.
    //
    return(0);
}

//*****************************************************************************
//
//! Handles messages for a widget.
//!
//! \param ulMsg is the message.
//! \param ulParam1 is the first parameter to the message.
//! \param ulParam2 is the second parameter to the message.
//!
//! This function must be redefined by derived class.
//!
//! \return Returns a value appropriate to the supplied message.
//
//*****************************************************************************
long Widget::MessageProc(unsigned long ulMessage, unsigned long ulParam1,
    		             unsigned long ulParam2)
{
	return(0);
}

//*****************************************************************************
//
//! Processes the messages in the widget message queue.
//!
//! This function extracts messages from the widget message queue one at a time
//! and processes them.  If the processing of a widget message requires that a
//! new message be sent, it is acceptable to call Widget::MessagePost().
//! It is also acceptable for code which interrupts this function to call
//! Widget::MessagePost() to send more messages.  In both cases, the newly
//! added message will also be processed before this function returns.
//!
//! \return None.
//
//*****************************************************************************
void
Widget::MessageQueueProcess(void)
{
	tMessageQueueItem sMessageQueueItem;

    //
    // Loop while there are more messages in the message queue.
    //
    while(Widget::pMQueue->DeQueue(&sMessageQueueItem))
    {
		//
		// Send this message to the widget.
		//
		Widget::MessageSend(sMessageQueueItem.pWidget,
							sMessageQueueItem.ulMessage,
							sMessageQueueItem.ulParam1,
							sMessageQueueItem.ulParam2,
							sMessageQueueItem.ulFlags);
    }
}

//*****************************************************************************
//
//! Destructor for a widget object.
//!
//! This function is a destructor for a widget object.
//!
//! \return None.
//
//*****************************************************************************
Widget::~Widget(void)
{

}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
