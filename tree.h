//*****************************************************************************
//
// tree.h - Prototypes for the tree class
//
//*****************************************************************************

#ifndef __TREE_H__
#define __TREE_H__

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
// Tree class
//
//*****************************************************************************
class Tree
{
public:
	//
	// Constructs a tree object
	//
	Tree();

	//
	// Gets a pointer to the parent of this tree
	//
	Tree *ParentGet()
	{
		return this->pParent;
	}

	//
	// Gets a pointer to the sibling of this tree
	//
	Tree *NextGet()
	{
		return this->pNext;
	}

	//
	// Gets a pointer to the child of this tree
	//
	Tree *ChildGet()
	{
		return this->pChild;
	}

	//
	// Appneds a child to this tree
	//
	void AppendChild(Tree *pChild);

	//
	// Cuts a tree
	//
	void Cut();

	//
	// Pre-order, depth-first search
	//
	Tree *PreOrderSearch(Tree *pLastSearch);

	//
	// Post-order, depth-first search
	//
	Tree *PostOrderSearch(Tree *pLastSearch);

	//
	// Destructs a tree object
	//
	virtual ~Tree();

private:
	//
	// A pointer to the parent of this tree
	//
	Tree *pParent;

	//
	// A pointer to the sibling of this tree
	//
	Tree *pNext;

	//
	// A pointer to the child of this tree
	//
	Tree *pChild;
};


//*****************************************************************************
//
// End of the C bindings section for C++ compilers
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* __TREE_H__ */
