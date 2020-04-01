//*****************************************************************************
//
// mqueue.cpp - Message queue implementation
//
//*****************************************************************************

#include "mqueue.h"

//*****************************************************************************
//
// Constructs a message queue object
//
//*****************************************************************************
MQueue::MQueue(unsigned long ulItemSize, unsigned long ulItemsCount)
{
	//
	// Set initial values
	//
	this->ulReadPtr    = 0;
	this->ulWritePtr   = 0;
	this->ucMutex      = 0;
	this->ulItemSize   = ulItemSize;
	this->ulItemsCount = ulItemsCount;

	//
	// Memory alloc for the queue
	//
	this->pMQ = new char[ulItemSize * ulItemsCount];
}

//*****************************************************************************
//
// Enqueues a new message
//
//*****************************************************************************
long MQueue::EnQueue(void *pMsgPtr)
{
	unsigned long ulWritePtrNext;
	unsigned long ulOffset;
			 long lRet = 0;

//	if(this->MutexGet())
	{
		//
		// Compute the next value for the write pointer
		//
		ulWritePtrNext = (this->ulWritePtr + 1) % this->ulItemsCount;

		//
		// Is the message queue full?
		//
		if(ulWritePtrNext != this->ulReadPtr)
		{
			//
			// Write this message into the next location in the message queue
			//
			ulOffset = this->ulWritePtr * this->ulItemSize;

			for(unsigned int i = 0; i < this->ulItemSize; i++)
			{
				*((char *)this->pMQ + ulOffset + i) = *((char *)pMsgPtr + i);
			}

			//
			// Update the message queue write pointer
			//
			this->ulWritePtr = ulWritePtrNext;

			//
			// Success
			//
			lRet = 1;
		}

		//
		// Release the message queue mutex
		//
//		this->MutexPut();
	}

	return lRet;
}

//*****************************************************************************
//
// Dequeues a message
//
//*****************************************************************************
long MQueue::DeQueue(void *pMsgPtr)
{
	unsigned long ulOffset;
			 long lRet = 0;

//	if(this->MutexGet())
	{
		//
		// Is any message present in queue?
		//
		if(this->ulReadPtr != this->ulWritePtr)
		{
			//
			// Copy the contents of this message
			//
			ulOffset = this->ulReadPtr * this->ulItemSize;

			for(unsigned int i = 0; i < this->ulItemSize; i++)
			{
				*((char *)pMsgPtr + i) = *((char *)this->pMQ + ulOffset + i);
			}

			//
			// Remove this message from the queue
			//
			this->ulReadPtr = (this->ulReadPtr + 1) % this->ulItemsCount;

			//
			// Success
			//
			lRet = 1;
		}

//		this->MutexPut();
	}

	return lRet;
}

//*****************************************************************************
//
// Shows a message from queue wihout dequeuing
//
//*****************************************************************************
long
MQueue::Show(void *pMsgPtr)
{
	unsigned long ulOffset;
			 long lRet = 0;

//	if(this->MutexGet())
	{
		//
		// Is any message present in queue?
		//
		if(this->ulReadPtr != this->ulWritePtr)
		{
			//
			// Copy the contents of this message
			//
			ulOffset = this->ulReadPtr * this->ulItemSize;

			for(unsigned int i = 0; i < this->ulItemSize; i++)
			{
				*((char *)pMsgPtr + i) = *((char *)this->pMQ + ulOffset + i);
			}

			//
			// Success
			//
			lRet = 1;
		}

//		this->MutexPut();
	}

	return lRet;
}

//*****************************************************************************
//
// Overwrites a message in queue
//
//*****************************************************************************
long
MQueue::OverWrite(void *pMsgPtr)
{
	unsigned long ulWritePtrPrev;
	unsigned long ulOffset;
			 long lRet = 0;

//	if(this->MutexGet())
	{
		//
		// Is the message queue empty?
		//
		if(this->ulReadPtr != this->ulWritePtr)
		{
			//
			// Compute the previous value for the write pointer
			//
			ulWritePtrPrev = (this->ulWritePtr == 0) ? (this->ulItemsCount - 1) : (this->ulWritePtr - 1);

			//
			// Overwrite a message into the previous location in the message queue
			//
			ulOffset = ulWritePtrPrev * this->ulItemSize;

			for(unsigned int i = 0; i < this->ulItemSize; i++)
			{
				*((char *)this->pMQ + ulOffset + i) = *((char *)pMsgPtr + i);
			}

			//
			// Success
			//
			lRet = 1;
		}

		//
		// Release the message queue mutex
		//
//		this->MutexPut();
	}

	return lRet;
}

//*****************************************************************************
//
// Attempts to acquire a mutex
//
//*****************************************************************************
long
MQueue::MutexGet(void)
{
	unsigned char ucExpected = 0;
	unsigned char ucDesired = 1;

	return __atomic_compare_exchange(&this->ucMutex, &ucExpected, &ucDesired, 0, 0, 0);
}

//*****************************************************************************
//
// Release a mutex
//
//*****************************************************************************
void
MQueue::MutexPut(void)
{
	this->ucMutex = 0;
}

//*****************************************************************************
//
// Destructs a message queue object
//
//*****************************************************************************
MQueue::~MQueue(void)
{
	delete[] (char *)this->pMQ;
}
