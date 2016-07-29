#pragma once
#ifndef __LINKED_TREE_H__
#define __LINKED_TREE_H__

template 
<
	typename T,
	u32 MaxChildren = MAXDWORD  // max num of children
>
class VGESS_API CLinkedTree
{
public:
	
	struct STreeNode
	{
		public:
			typedef STreeNode* SelfPos;
			DEFINE_VECTOR(CHILDREN_STORAGE, SelfPos);
			typedef CHILDREN_STORAGE_IT iterator;
			typedef CHILDREN_STORAGE_CIT const_iterator; 
		public:
			STreeNode(SelfPos p = 0) :
				m_parent(p)
			{
				Init_internal();
			}
			STreeNode(T element, SelfPos p = 0) :
				m_parent(p), m_element(element)
			{
				Init_internal();
			}
		private:
			void Init_internal()
			{
				m_children_count = 0;
				if (MaxChildren < MAXDWORD)
				{
				//	m_children.assign(MaxChildren, (SelfPos) 0);
				}
			}
		public:
			virtual ~STreeNode()
			{
				DetachChildren();
			}
			void DetachChildren()
			{
				for (CHILDREN_STORAGE_IT it = begin();
						it != end();
						++it)
				{
					(*it)->DetachChildren();
					Delete(*it);
				}
				m_children.clear();
			}
			T getElement() const
			{
				return m_element;
			}
			void setElement(T elem)
			{
				m_element = elem;
			}
			iterator begin()
			{
				return m_children.begin();
			}
			const_iterator begin() const
			{
				return m_children.begin();
			}
			iterator end()
			{
				return m_children.end();
			}
			const_iterator end() const
			{
				return m_children.end();
			}
			SelfPos operator[] (int i)
			{
				return m_children[i];
			}
			SelfPos getChild(u32 i)
			{
				if (i >= m_children.size())
					return NULL;
				return m_children[i];
			}
			virtual SelfPos addChild(T elem)
			{
				SelfPos ret = New<STreeNode>(elem, this);
				m_children.push_back(ret);
				m_children_count++;
				return ret;
			}
			SelfPos getParent() const
			{
				return m_parent;
			}
			void setParent(SelfPos p)
			{
				m_parent = p;
			}
			u32 getChildrenCount() const
			{
				return m_children_count;
			}
		protected:
			T					m_element;
			CHILDREN_STORAGE	m_children;
			SelfPos				m_parent;
			u32					m_children_count;

	};

public:
	CLinkedTree() : m_size(0), m_root(0)
	{
	}
	CLinkedTree(T elem) : m_size(0)
	{
		insertRoot(elem);
	}
	virtual ~CLinkedTree()
	{
		Delete(m_root);
	}
	virtual STreeNode* root()
	{
		return m_root;
	}
	virtual STreeNode* parent(STreeNode* child)
	{
		ASSERT(child, "invalid node");
		if (isRoot(child))
			return NULL;
		return child->getParent();
	}
	bool isRoot(STreeNode* node)
	{
		return m_root == node;
	}
	virtual STreeNode* insertRoot(T elem)
	{
		return (m_root = New<STreeNode>(elem));
	}
	

	virtual STreeNode* insert(STreeNode* parent, T elem)
	{
		if (parent)
		{
			ASSERT(parent->getChildrenCount() <= MaxChildren, "max children");
			m_size++;
			return parent->addChild(elem);
		}
		else if (!m_root)
		{
			m_size++;
			return insertRoot(elem);
		}
		FATAL("invalid parent");
		return NULL;
	}
#if 0
	string toString(STreeNode* start)
	{
		ASSERT(start, "invalid node");
		string str = format("0x%0x child of 0x%0x\n", start, parent(start));
		for (STreeNode::iterator it = start->begin();
				it != start->end();
				++it)
		{
			str += toString(*it);
			str += "\n";
		}
		return str;
	}
#else
	struct VoidOperation 
	{
		string operator() (STreeNode* node)
		{
			return "";
		}
	};
	
	template <typename Operation>
	string toString(STreeNode* start, Operation o = void_operation)
	{
		ASSERT(start, "invalid node");
		STreeNode* p = parent(start);
		string str = format("[0x%08x][%s] child of [0x%08x]\n", start, *(o(start)), p);
		for (STreeNode::iterator it = start->begin();
				it != start->end();
				++it)
		{
			str += toString(*it, o);
			str += "\n";
		}
		return str;
	}
#endif
	u32 Size() const
	{
		return m_size;
	}

	bool isEmpty() const
	{
		return m_size == 0;
	}

protected:

	STreeNode*	m_root;
	u32			m_size;
};

template
<
	typename T,
	u32 BinaryChildren = 2
>
class VGESS_API CLinkedBinaryTree : public CLinkedTree<T, BinaryChildren>
{

public:

	struct SBTreeNode : public CLinkedTree<T, BinaryChildren>::STreeNode
	{
		public:

			typedef CLinkedTree<T, BinaryChildren>::STreeNode Base;

			SBTreeNode(SelfPos p = 0, u32 idx = 1) :
				Base(p), m_index(idx)
			{
				
			}
			SBTreeNode(T element, SelfPos p = 0, u32 idx = 1) :
				Base(element, p), m_index(idx)
			{
				
			}
			u32 getIndex() const
			{
				return m_index;
			}
			void setIndex(u32 idx)
			{
				m_index = idx;
			}

			void swapChildren()
			{
				std::swap(m_children.begin(), m_children.begin() + 1);
			}

			virtual SBTreeNode* addChild(T elem)
			{
				SBTreeNode* ret = New<SBTreeNode>(elem, this);
				m_children.push_back(ret);
				m_children_count++;
				return ret;
			}

		private:
			u32		m_index;
	};

private:
	typedef CLinkedTree<T, BinaryChildren>	Base;
	enum 
	{
		eLeftChild = 0,
		eRightChild,
	};
public:
	CLinkedBinaryTree() : Base()
	{
	}

	CLinkedBinaryTree(T elem) : Base(elem)
	{
	}

	virtual ~CLinkedBinaryTree()
	{
	}

	bool hasLeft(STreeNode* node)
	{
		ASSERT(checkNode(node), "invalid node");
		return node->getChild(eLeftChild) != NULL;
	}

	bool hasRight(STreeNode* node)
	{
		ASSERT(checkNode(node), "invalid node");
		return node->getChild(eRightChild) != NULL;
	}

	SBTreeNode* left(STreeNode* node)
	{
		ASSERT(checkNode(node), "invalid node");
		return checkNode(node->getChild(eLeftChild));
	}

	SBTreeNode* right(STreeNode* node)
	{
		ASSERT(checkNode(node), "invalid node");
		return checkNode(node->getChild(eRightChild));
	}

	STreeNode* insertLeft(STreeNode* parent, T e)
	{
		SBTreeNode* w = checkNode(parent);
		ASSERT(w, "invalid node");
		ASSERT(!hasLeft(w), "parent has already left");
		SBTreeNode *node = checkNode(Base::insert(w, e));
		node->setIndex(w->getIndex() * 2);
		return node;
	}

	STreeNode* insertRight(STreeNode* parent, T e)
	{
		SBTreeNode* w = checkNode(parent);
		ASSERT(w, "invalid node");
		ASSERT(!!hasLeft(w), "parent has not left");
		ASSERT(!hasRight(w), "parent has already right");
		SBTreeNode *node = checkNode(Base::insert(w, e));
		node->setIndex(w->getIndex() * 2 + 1);
		return node;
	}

	virtual STreeNode* insert(STreeNode* parent, T elem)
	{
		if (parent)
		{
			if (!hasLeft(parent))
				return insertLeft(parent, elem);
			if (!hasRight(parent))
				return insertRight(parent, elem);
		}
		else if (!m_root)
		{
			m_size++;
			return insertRoot(elem);
		}
		FATAL("invalid parent");
		return NULL;
	}

	virtual STreeNode* insertRoot(T elem)
	{
		return (m_root = New<SBTreeNode>(elem));
	}

	u32 getIndex(STreeNode* node)
	{
		return checkNode(node)->getIndex();
	}

	u32 height(STreeNode* v) 
	{
		if (isExternal(v))
		{
			return 0;
		}
		else
		{
			u32 lh = hasLeft(v) ? height(left(v)) : 0;
			u32 rh = hasRight(v) ? height(right(v)) : 0;
			return max(lh, rh) + 1;
		}
	}
	
	u32 depth(STreeNode* v) 
	{
		if (isRoot(v))
		{
			return 0;
		}
		else
		{
			return 1 + depth(parent(v));
		}
	}
	
	bool isInternal(STreeNode* v)
	{
		return hasLeft(v) || hasRight(v);
	}

	bool isExternal(STreeNode* v)  
	{
		return !isInternal(v);
	}

	SBTreeNode* checkNode(STreeNode* node)
	{
		SBTreeNode* w = dynamic_cast<SBTreeNode*>(node);
		ASSERT(w, "invalid node");
		return w;
	}
	
	u32 getLeavesCount(STreeNode* v)
	{
		SBTreeNode* w = checkNode(v);
		if (isExternal(w))
		{
			return 1;
		}
		else
		{
			u32 l, r;
			l = r = 0;
			if (hasLeft(w))
				l = getLeavesCount(left(w));
			if (hasRight(w))
				r = getLeavesCount(right(w));
			return l + r;
		}
	}

	void getLeaves(CVector<SBTreeNode*>& leaves, STreeNode* v)
	{
		SBTreeNode* w = checkNode(v);
		if (isExternal(w))
		{
			leaves.push_back(w);
		}
		else
		{
			if (hasLeft(w))
				getLeaves(leaves, left(w));
			if (hasRight(w))
				getLeaves(leaves, right(w));
		}
	}

	void mirror(STreeNode* v)
	{
		SBTreeNode* w = checkNode(v);
		if (!isExternal(w))
		{
			SBTreeNode* l = left(w);
			SBTreeNode* r = right(w);
			w->swapChildren();
			mirror(l);
			mirror(r);
		}
	}

	template <typename Operation>
	void postOrderVisit(STreeNode* node, const Operation& oper)
	{
		SBTreeNode* w = checkNode(node);
		if (isExternal(w))
		{
			oper(w);
		}
		else
		{
			if (hasLeft(w))
				postOrderVisit(left(w), oper);
			if (hasRight(w))
				postOrderVisit(right(w), oper);
			oper(w);

		}
	}

	template <typename Operation>
	void preOrderVisit(STreeNode* node, const Operation& oper)
	{
		if (isExternal(node))
		{
			oper(node);
		}
		else
		{
			oper(node);
			if (hasLeft(node))
				postOrderVisit(left(node), oper);
			if (hasRight(node))
				postOrderVisit(right(node), oper);
		}
	}

	template <typename Operation>
	void inOrderVisit(STreeNode* node, const Operation& oper)
	{
		if (isExternal(node))
		{
			oper(node);
		}
		else
		{
			if (hasLeft(node))
				postOrderVisit(left(node), oper);
			oper(node);
			if (hasRight(node))
				postOrderVisit(right(node), oper);
		}
	}

	template <typename Operation>
	void bfsVisit(const Operation& oper)
	{
		CAssocVector<u32, CVector<SBTreeNode*>> layers;
		u16 layer = 0;
		layers[layer].push_back(root());
		oper(layer, layers[layer].back());
		while (layers[layer].size())
		{
			for (u32 i = 0; i < layers[layer].size(); i++)
			{
				SBTreeNode* v = checkNode(layers[layer][i]);
				u32 children_count = v->getChildrenCount();
				for (u32 j = 0; j < children_count; j++)
				{
					SBTreeNode* w = checkNode(v->getChild(j));
					oper(layer + 1, w);	
					layers[layer + 1].push_back(w);
				}
			}
			layer++;
		}
	}

	virtual SBTreeNode* root()
	{
		return checkNode(m_root);
	}
	virtual SBTreeNode* parent(STreeNode* child)
	{
		ASSERT(child, "invalid node");
		if (isRoot(child))
			return NULL;
		return checkNode(child->getParent());
	}

};

#endif
