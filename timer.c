//*****************************************************************************
//
// timer.c - Routines for timer.
//
//*****************************************************************************

#include "hw_types.h"
#include "debug.h"
#include "grlib.h"

//*****************************************************************************
//
//! \addtogroup timer_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// The milisecons counter
//
//*****************************************************************************
static unsigned long g_ulTimer1msCounter;

//*****************************************************************************
//
//! Increments timer
//!
//! This function increments timer value. It must be called erey 1ms from
//! client program
//!
//! \return None.
//
//*****************************************************************************
void
GrTimerInc(void)
{
	g_ulTimer1msCounter++;
}

//*****************************************************************************
//
//! Gets the timer value
//!
//! This function gets the timer value
//!
//! \return None.
//
//*****************************************************************************
unsigned long
GrTimerGet(void)
{
	return g_ulTimer1msCounter;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
