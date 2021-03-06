//*****************************************************************************
//
// circularbutton.h - Prototypes for the circular button.
//
//*****************************************************************************

#ifndef __CIRCULARBUTTON_H__
#define __CIRCULARBUTTON_H__

//*****************************************************************************
//
//! \addtogroup circularbutton_api
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
//! The class that describes a circular button widget.
//
//*****************************************************************************
class CircularButton : public Widget
{
public:
	//
	//! Constructs a CircularButton widget
	//
	CircularButton(unsigned char ucID, Widget *pParent, long lX, long lY, long lR);

	//
	//! Draws the contents of a circular button.
	//
	void Draw(void);

	//
	//! Handles pointer events for a circular button.
	//
	long Click(unsigned long ulMsg, long lX, long lY);

	//
	//! Handles messages for a circular button widget.
	//
	long MessageProc(unsigned long ulMsg, unsigned long ulParam1,
			         unsigned long ulParam2);

	//
	//! Destroys a CircularButton widget
	//
	~CircularButton(void);

    //
    //! The style for this widget.  This is a set of flags defined by
    //! CB_STYLE_xxx.
    //
    unsigned long ulStyle;

    //
    //! The 24-bit RGB color used to fill this circular button, if CB_STYLE_FILL is
    //! selected, and to use as the background color if CB_STYLE_TEXT_OPAQUE is
    //! selected.
    //
    unsigned long ulFillColor;

    //
    //! The 24-bit RGB color used to fill this circular button when it is pressed,
    //! if CB_STYLE_FILL is selected, and to use as the background color if
    //! CB_STYLE_TEXT_OPAQUE is selected.
    //
    unsigned long ulPressFillColor;

    //
    //! The 24-bit RGB color used to outline this circular button, if
    //! CB_STYLE_OUTLINE is selected.
    //
    unsigned long ulOutlineColor;

    //
    //! The 24-bit RGB color used to draw text on this circular button, if
    //! CB_STYLE_TEXT is selected.
    //
    unsigned long ulTextColor;

    //
    //! A pointer to the font used to render the circular button text, if
    //! CB_STYLE_TEXT is selected.
    //
    const tFont *pFont;

    //
    //! A pointer to the text to draw on this circular button, if CB_STYLE_TEXT is
    //! selected.
    //
    char *pcText;

    //
    //! A pointer to the image to be drawn onto this circular button, if
    //! CB_STYLE_IMG is selected.
    //
    const unsigned char *pucImage;

    //
    //! A pointer to the image to be drawn onto this circular button when it is
    //! pressed, if CB_STYLE_IMG is selected.
    //
    const unsigned char *pucPressImage;

    //
    //! The number of pointer events to delay before starting to auto-repeat,
    //! if CB_STYLE_AUTO_REPEAT is selected.  The amount of time to which this
    //! corresponds is dependent upon the rate at which pointer events are
    //! generated by the pointer driver.
    //
    unsigned short usAutoRepeatDelay;

    //
    //! The number of pointer events between button presses generated by the
    //! auto-repeat function, if CB_STYLE_AUTO_REPEAT is selected.  The amount
    //! of time to which this corresponds is dependent up on the rate at which
    //! pointer events are generated by the pointer driver.
    //
    unsigned short usAutoRepeatRate;

    //
    //! The number of pointer events that have occurred.  This is used when
    //! CB_STYLE_AUTO_REPEAT is selected to generate the auto-repeat events.
    //
    unsigned long ulAutoRepeatTimer;

    //
    //! Indicate that button is in autorepeat state.
    //
    unsigned char bAutoRepeat;
};

//*****************************************************************************
//
//! This flag indicates that the circular button should be outlined.
//
//*****************************************************************************
#define CB_STYLE_OUTLINE        0x00000001

//*****************************************************************************
//
//! This flag indicates that the circular button should be filled.
//
//*****************************************************************************
#define CB_STYLE_FILL           0x00000002

//*****************************************************************************
//
//! This flag indicates that the circular button should have text drawn on it.
//
//*****************************************************************************
#define CB_STYLE_TEXT           0x00000004

//*****************************************************************************
//
//! This flag indicates that the circular button should have an image drawn on it.
//
//*****************************************************************************
#define CB_STYLE_IMG            0x00000008

//*****************************************************************************
//
//! This flag indicates that the circular button text should be drawn opaque (in
//! other words, drawing the background pixels as well as the foreground
//! pixels).
//
//*****************************************************************************
#define CB_STYLE_TEXT_OPAQUE    0x00000010

//*****************************************************************************
//
//! This flag indicates that the circular button should auto-repeat, generating
//! repeated click events while it is pressed.
//
//*****************************************************************************
#define CB_STYLE_AUTO_REPEAT    0x00000020

//*****************************************************************************
//
//! This flag indicates that the circular button is pressed.
//
//*****************************************************************************
#define CB_STYLE_PRESSED        0x00000040

//*****************************************************************************
//
//! This flag indicates that the circular button is disabled
//! the button fills by gray and no click events processed.
//
//*****************************************************************************
#define CB_STYLE_DISABLED       0x00000080

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

#endif //__CIRCULARBUTTON_H__
