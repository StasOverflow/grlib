//*****************************************************************************
//
// mqueue.h - Prototypes for the message queue class
//
//*****************************************************************************

#ifndef __MQUEUE_H__
#define __MQUEUE_H__

//*****************************************************************************
//
// Start of the C bindings section for C++ compilers
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Message queue class
//
//*****************************************************************************
class MQueue
{
public:
	//
	// Constructs a message queue object
	//
	MQueue(unsigned long ulItemSize, unsigned long ulItemsCount);

	//
	// Enqueues a new message
	//
	long EnQueue(void *pMsgPtr);

	//
	// Dequeues a message
	//
	long DeQueue(void *pMsgPtr);

	//
	// Shows a message from queue wihout dequeuing
	//
	long Show(void *pMsgPtr);

	//
	// Overwrites a message in queue
	//
	long OverWrite(void *pMsgPtr);

	//
	// Destructs a message queue object
	//
	~MQueue(void);

private:
	//
	// Attempts to acquire a mutex
	//
	long MutexGet(void);

	//
	// Release a mutex
	//
	void MutexPut(void);

	//
	// Pointer to the message queue
	//
	void *pMQ;

	//
	// Size of the message item in queue
	//
	unsigned long ulItemSize;

	//
	// Number of all messages to be queued
	//
	unsigned long ulItemsCount;

	//
	// The index of a message to be read from the message queue
	//
	unsigned long ulReadPtr;

	//
	// The index of a message to be written to the message queue
	//
	unsigned long ulWritePtr;

	//
	// The mutex that used to protect access to the message queue
	//
	unsigned char ucMutex;
};

//*****************************************************************************
//
// End of the C bindings section for C++ compilers
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* __MQUEUE_H__ */
