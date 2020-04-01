//*****************************************************************************
//
// widget.h - Prototypes for the widget base class.
//
//*****************************************************************************

#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "tree.h"
#include "mqueue.h"
#include "grlib.h"

//*****************************************************************************
//
//! \addtogroup widget_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif


//*****************************************************************************
//
//! This class describes a generic widget. It's a base class for all other
//! widgets.
//
//*****************************************************************************
class Widget : public Tree
{
public:
    //
    //! Constructor.
    //
	Widget(unsigned char ucID, Widget *pParent, long lX, long lY, long lWidth, long lHeight);

	//
	//! Updates this widget and all his children.
	//
	void Update(void);

	//
	//! Moves this widget on display.
	//
	void Move(long lX, long lY);

	//
	//! Flushes a widget to the display.
	//
	void Flush(void);

	//
	//! Gets an absolute coordinates of this widget.
	//
	void AbsolutePositionGet(tRectangle *psPosition);

    //
    //! Destroys this widget and all his children.
    //
	void Destroy(void);

	//
	//! Adds message to the widget message queue.
	//
	static long MessagePost(Widget *pWidget,
			                unsigned long ulMessage,
			                unsigned long ulParam1,
			                unsigned long ulParam2,
			                unsigned long ulFlags);

	//
	//! Sends a message to a widget tree.
	//
	static long MessageSend(Widget *pWidget,
			                unsigned long ulMessage,
			                unsigned long ulParam1,
			                unsigned long ulParam2,
			                unsigned long ulFlags);

	//
	//! Sends notification message to parent
	//
	static long ParentNotify(Widget *pWidget,
                             unsigned long ulMessage);

	//
	//! Process a message queue.
	//
	static void MessageQueueProcess(void);

    //
    //! The procedure that handles messages sent to this widget.
    //
	virtual long MessageProc(unsigned long ulMessage,
			                 unsigned long ulParam1,
    		                 unsigned long ulParam2);

    //
    //! Destructor.
    //
	virtual ~Widget(void);

private:
	//
	//! An ID of this widget.
	//
	unsigned char ucID;

    //
    //! The rectangle that encloses this widget.
    //
    tRectangle sPosition;

    //
    //! The pointer to the message queue object.
    //
    static MQueue *pMQueue;

	//
	//! Sends a message to a widget tree via a pre-order, depth-first search.
	//
	static long MessageSendPreOrder(Widget *pWidget, unsigned long ulMessage,
	                                unsigned long ulParam1,
	                                unsigned long ulParam2,
	                                char bStopOnSuccess,
	                                char bStopAfterFirst);

	//
	//! Sends a message to a widget tree via a post-order, depth-first search.
	//
	static long MessageSendPostOrder(Widget *pWidget, unsigned long ulMessage,
	                                 unsigned long ulParam1,
	                                 unsigned long ulParam2,
	                                 char bStopOnSuccess,
	                                 char bStopAfterFirst);
};

//*****************************************************************************
//
// Flags that indicate how messages from the message queue are processed.  They
// can be sent via either a pre-order or post-order search, and can optionally
// be sent to no other widgets once one accepts the message.
//
//*****************************************************************************
#define MSG_FLAG_POST_ORDER         0x00000001
#define MSG_FLAG_STOP_ON_SUCCESS    0x00000002
#define MSG_FLAG_STOP_ON_FIRST      0x00000004

//*****************************************************************************
//
// Prototypes for the generic widget handling functions.
//
//*****************************************************************************
extern Widget *g_pcRootWidget;

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

#endif // __WIDGET_H__
