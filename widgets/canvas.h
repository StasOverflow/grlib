//*****************************************************************************
//
// canvas.h - Prototypes for the canvas widget.
//
//*****************************************************************************

#ifndef __CANVAS_H__
#define __CANVAS_H__

//*****************************************************************************
//
//! \addtogroup canvas_api
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
//! The class that describes a canvas widget.
//
//*****************************************************************************
class Canvas : public Widget
{
public:
	//
	//! Constructs a canvas widget
	//
	Canvas(unsigned char ucID, Widget *pParent, long lX, long lY, long lWidth, long lHeight);

	//
	//! Draws the contents of a canvas.
	//
	void Draw(void);

	//
	//! Handles messages for a canvas widget.
	//
	long MessageProc(unsigned long ulMsg, unsigned long ulParam1,
			         unsigned long ulParam2);

	//
	//! Destroys a canvas widget
	//
	~Canvas(void);

    //
    //! The style for this widget.  This is a set of flags defined by
    //! CANVAS_STYLE_xxx.
    //
    unsigned long ulStyle;

    //
    //! The 24-bit RGB color used to fill this canvas, if CANVAS_STYLE_FILL is
    //! selected, and to use as the background color if
    //! CANVAS_STYLE_TEXT_OPAQUE is selected.
    //
    unsigned long ulFillColor;

    //
    //! The first 24-bit RGB color used to fill by gradient this canvas,
    //!  if CANVAS_STYLE_GRADIENT is selected.
    //
    unsigned long ulGradientColor1;

    //
    //! The second 24-bit RGB color used to fill by gradient this canvas,
    //!  if CANVAS_STYLE_GRADIENT is selected.
    //
    unsigned long ulGradientColor2;

    //
    //! The 24-bit RGB color used to outline this canvas, if
    //! CANVAS_STYLE_OUTLINE is selected.
    //
    unsigned long ulOutlineColor;

    //
    //! The 24-bit RGB color used to draw text on this canvas, if
    //! CANVAS_STYLE_TEXT is selected.
    //
    unsigned long ulTextColor;

    //
    //! A pointer to the font used to render the canvas text, if
    //! CANVAS_STYLE_TEXT is selected.
    //
    const tFont *pFont;

    //
    //! A pointer to the text to draw on this canvas, if CANVAS_STYLE_TEXT is
    //! selected.
    //
    char *pcText;

    //
    //! A pointer to the image to be drawn onto this canvas, if
    //! CANVAS_STYLE_IMG is selected.
    //
    const unsigned char *pucImage;
};

//*****************************************************************************
//
//! This flag indicates that the canvas should be filled by gradient.
//
//*****************************************************************************
#define CANVAS_STYLE_GRADIENT          0x00000001

//*****************************************************************************
//
//! This flag indicates that the canvas should be outlined.
//
//*****************************************************************************
#define CANVAS_STYLE_OUTLINE           0x00000002

//*****************************************************************************
//
//! This flag indicates that the canvas should be filled.
//
//*****************************************************************************
#define CANVAS_STYLE_FILL              0x00000004

//*****************************************************************************
//
//! This flag indicates that the canvas has a rising edge.
//
//*****************************************************************************
#define CANVAS_STYLE_RISING_EDGE       0x00000008

//*****************************************************************************
//
//! This flag indicates that the canvas has a falling edge.
//
//*****************************************************************************
#define CANVAS_STYLE_FALLING_EDGE      0x00000010

//*****************************************************************************
//
//! This flag indicates that the canvas should have text drawn on it.
//
//*****************************************************************************
#define CANVAS_STYLE_TEXT              0x00000020

//*****************************************************************************
//
//! This flag indicates that the canvas should have an image drawn on it.
//
//*****************************************************************************
#define CANVAS_STYLE_IMG               0x00000040

//*****************************************************************************
//
//! This flag indicates that the canvas should send a WIDGET_PAINT message
//! to his parent when it finish draw itself.
//
//*****************************************************************************
#define CANVAS_STYLE_PAINT_NOTIFY      0x00000040

//*****************************************************************************
//
//! This flag indicates that the canvas text should be drawn opaque (in other
//! words, drawing the background pixels as well as the foreground pixels).
//
//*****************************************************************************
#define CANVAS_STYLE_TEXT_OPAQUE       0x00000080

//*****************************************************************************
//
//! This flag indicates that canvas text should be left-aligned. By default,
//! text is centered in both X and Y within the canvas bounding rectangle.
//
//*****************************************************************************
#define CANVAS_STYLE_TEXT_LEFT         0x00000100

//*****************************************************************************
//
//! This flag indicates that canvas text should be right-aligned. By default,
//! text is centered in both X and Y within the canvas bounding rectangle.
//
//*****************************************************************************
#define CANVAS_STYLE_TEXT_RIGHT        0x00000200

//*****************************************************************************
//
//! This flag indicates that canvas text should be top-aligned. By default,
//! text is centered in both X and Y within the canvas bounding rectangle.
//
//*****************************************************************************
#define CANVAS_STYLE_TEXT_TOP          0x00000400

//*****************************************************************************
//
//! This flag indicates that canvas text should be bottom-aligned. By default,
//! text is centered in both X and Y within the canvas bounding rectangle.
//
//*****************************************************************************
#define CANVAS_STYLE_TEXT_BOTTOM       0x00000800

//*****************************************************************************
//
//! This flag indicates that canvas text should be centered horizontally. By
//! default, text is centered in both X and Y within the canvas bounding
//! rectangle.
//
//*****************************************************************************
#define CANVAS_STYLE_TEXT_HCENTER      0x00000000

//*****************************************************************************
//
//! This flag indicates that canvas text should be centered vertically. By
//! default, text is centered in both X and Y within the canvas bounding
//! rectangle.
//
//*****************************************************************************
#define CANVAS_STYLE_TEXT_VCENTER      0x00000000

//*****************************************************************************
//
// Masks used to extract the text alignment flags from the widget style.
//
//*****************************************************************************
#define CANVAS_STYLE_ALIGN_MASK (CANVAS_STYLE_TEXT_LEFT |   \
                                 CANVAS_STYLE_TEXT_RIGHT |  \
                                 CANVAS_STYLE_TEXT_TOP |    \
                                 CANVAS_STYLE_TEXT_BOTTOM)
#define CANVAS_STYLE_ALIGN_HMASK \
                                (CANVAS_STYLE_TEXT_LEFT |   \
                                 CANVAS_STYLE_TEXT_RIGHT)
#define CANVAS_STYLE_ALIGN_VMASK \
                                (CANVAS_STYLE_TEXT_TOP |    \
                                 CANVAS_STYLE_TEXT_BOTTOM)

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

#endif // __CANVAS_H__
