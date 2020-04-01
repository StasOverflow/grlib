//*****************************************************************************
//
// tree.cpp - Tree class implementation
//
//*****************************************************************************

#include "tree.h"

//*****************************************************************************
//
// Constructs a tree object
//
//*****************************************************************************
Tree::Tree()
{
	//
	// Set initial values
	//
	this->pParent = 0;
	this->pNext   = 0;
	this->pChild  = 0;
}

//*****************************************************************************
//
// Appneds a child to this tree
//
//*****************************************************************************
void
Tree::AppendChild(Tree *pChild)
{
	Tree *pTemp;

    //
    // Make this tree be a parent of our child that to be appended
    //
    pChild->pParent = this;

    //
    // See if this tree already has children
    //
    if(this->pChild)
    {
        //
        // Find the last child of this tree and also check that our child
    	// is not already present at this level of the tree
        //
        for(pTemp = this->pChild; pTemp->pNext; pTemp = pTemp->pNext)
        {
            //
            // Our child is allready present in the tree, just return
            //
            if(pTemp == pChild)
            {
                return;
            }
        }

        //
        // Check again
        //
        if(pTemp == pChild)
        {
            return;
        }

        //
        // Add our child to the end of the list of children of this tree
        //
        pTemp->pNext = pChild;
    }
    else
    {
        //
        // Make our child be the first (and only) child of this tree
        //
        this->pChild = pChild;
    }
}

//*****************************************************************************
//
// Cuts a tree
//
//*****************************************************************************
void
Tree::Cut()
{
    Tree *pTemp;

    //
    // Make sure that this tree has a parent
    //
    if(this->pParent)
    {
        //
        // See if this tree is the first child of its parent
        //
        if(this->pParent->pChild == this)
        {
            //
            // Make the first child of this tree's parent be this tree's
            // sibling
            //
            this->pParent->pChild = this->pNext;
        }
        else
        {
            //
            // Find the sibling directly before this tree
            //
            for(pTemp = this->pParent->pChild; pTemp->pNext != this;
                pTemp = pTemp->pNext)
            {
            }

            //
            // Make the previous sibling point to the next sibling, removing
            // this tree from the sibling chain
            //
            pTemp->pNext = this->pNext;
        }

        //
        // Clear the parent and next point
        //
        this->pParent = 0;
        this->pNext = 0;
    }
}

//*****************************************************************************
//
// Pre-order, depth-first search
//
//*****************************************************************************
Tree*
Tree::PreOrderSearch(Tree *pLastSearch)
{
	Tree *pTemp;

	//
	// See if this is a first search
	//
    if(!pLastSearch)
    {
    	return this;
    }
    else
    {
    	pTemp = pLastSearch;
    }

	//
	// See if this tree has a child
	//
	if(pTemp->pChild)
	{
		//
		// Visit to child first
		//
		return pTemp->pChild;
	}
	else
	{
		//
		// Loop until returning to the starting tree
		//
		while(pTemp != this)
		{
			//
			// See if this tree has a sibling
			//
			if(pTemp->pNext)
			{
				//
				// Visit the sibling of this tree
				//
				return pTemp->pNext;
			}
			else
			{
				//
				// This tree has no siblings, so go to its parent
				//
				pTemp = pTemp->pParent;
			}
		}
	}

    //
    // Search finished, no any trees found
    //
    return 0;
}

//*****************************************************************************
//
// Post-order, depth-first search
//
//*****************************************************************************
Tree*
Tree::PostOrderSearch(Tree *pLastSearch)
{
	Tree *pTemp;
	Tree *pPrev;

	//
	// See if this is a first search
	//
    if(!pLastSearch)
    {
    	pTemp = this;

		//
		// Find a deepest tree
		//
		while(pTemp->pChild)
		{
			//
			// Go to this tree's child first.
			//
			pTemp = pTemp->pChild;

			//
			// Go to the last siblig tree at this level
			//
			while(pTemp->pNext)
			{
				pTemp = pTemp->pNext;
			}
		}

		return pTemp;
    }
    else
    {
    	pTemp = pLastSearch;
    }

    //
    // Check for search complete
    //
    if(pTemp != this)
    {
    	//
    	// Check if this tree is not a first child of his parent
    	//
    	if((pTemp->pParent) && (pTemp->pParent->pChild != pTemp))
    	{
    		//
    		// Find the sibling directly before this tree (previous)
    		//
    		for(pPrev = pTemp->pParent->pChild; pPrev->pNext != pTemp;
    			pPrev = pPrev->pNext)
    		{
    		}

    		//
    		// Visit to the previous sibling of this tree
    		//
    		pTemp = pPrev;

    		//
    		// Find a deepest tree
    		//
    		while(pTemp->pChild)
    		{
    			//
    			// Go to this tree's child first
    			//
    			pTemp = pTemp->pChild;

    			//
    			// Go to the last siblig tree at this level
    			//
    			while(pTemp->pNext)
    			{
    				pTemp = pTemp->pNext;
    			}
    		}

    		return pTemp;
    	}
    	else
    	{
    		//
    		// This tree has no siblings, so go to its parent
    		//
    		return pTemp->pParent;
    	}
    }

    //
    // Search finished, no any trees found
    //
    return 0;
}

//*****************************************************************************
//
// Destructs a tree object
//
//*****************************************************************************
Tree::~Tree()
{

}
