/*
	file name: BinaryTree.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 11/04/2013
*/

#pragma once
#ifndef __BINARY_TREE_H
#define __BINARY_TREE_H
#include <cmath>

template <typename T> struct BTPosition
{
	private:
		u32 position;
		T element;
	public:
		BTPosition (T e, u32 p) : element(e), position(p)
		{
			//
		}

		T getElement ()
		{
			return element;
		}

		T operator* ()
		{
			return getElement();
		}

		u32	getIndex () const
		{
			return position;
		}

		void setIndex (u32 p)
		{
			position = p;
		}
		T setElement (T e)
		{
			T tmp	 =	 element;
			element  =	 e;
			return   tmp;
		}		
};

template <typename T>
inline bool operator== (const BTPosition<T>& a, const BTPosition<T>& b)
{
	return a.getIndex() == b.getIndex();
}

template <typename T>
inline bool operator!= (const BTPosition<T>& a, const BTPosition<T>& b)
{
	return !(a == b);
}

template <typename T, int default_capacity = 32> class CBinaryTree
{
	public:
		typedef BTPosition<T>						TBTPosition;
		
	protected:
		CVector<TBTPosition*>	tree;
		u32						size;
		u32						current_capacity;

	public:
		CBinaryTree			 () :
			size(0), current_capacity(default_capacity), tree(default_capacity, (TBTPosition*) NULL)
		{
		}
		~CBinaryTree		()
		{
			tree.clear();
		}

		/**
			Return left position 
		*/
		TBTPosition* left (TBTPosition* p)
		{
			if(!hasLeft(p))
				return NULL;
			return tree[2*p->getIndex()];
		}

		/**
			Return right position
		*/
		TBTPosition* right (TBTPosition* p)
		{
			if(!hasRight(p))
				return NULL;
			return tree[2*p->getIndex() + 1];
		}

		/**
			Return false if p has not left child, true otherwise
		*/
		bool hasLeft (TBTPosition* p)
		{
			return 2 * p->getIndex() <= size;
		}

		/**
			Return false if p has not right child, true otherwise
		*/
		bool hasRight (TBTPosition* p)
		{
			return 2 * p->getIndex() + 1 <= size;
		} 

		/**
			Return true if tree is empty, false otherwise
		*/
		bool isEmpty ()
		{
//			Log("%d size albero!", size);
			return size <= 1;
		}

		u32 Size() const
		{
			return size - 1;
		}

		/**
			Return position with the new element
		*/
		T replace (TBTPosition* p, T e)
		{
			T tmp = p->getElement();
			p->setElement(e);
			return tmp;
		}

		/**
			Return root of tree
		*/
		TBTPosition* root ()
		{
			if ( isEmpty() )
			{
				Log ("error root");
				return NULL;
			}
			return tree[1];
		}
		
		/**
			Return parent of p
		*/
		TBTPosition* parent(TBTPosition* p)
		{
			if ( isRoot(p) )
			{
				Log ( "parent not present");
				return NULL;
			}
			return tree[p->getIndex()/2];
		}

		/**
			Return the position of tree with idx
		*/
		TBTPosition* access(u32 idx = 1)
		{
			ASSERT(idx && idx <= size, "wrong idx");
			return tree[idx];
		}

		/**
			Return true if p is internal node, false otherwise
		*/
		bool isInternal (TBTPosition* p)
		{
			return p != NULL && hasLeft(p);
		}

		/**
			Return true is p is external node, false otherwise
		*/
		bool isExternal (TBTPosition* p)
		{
			return p != NULL && !hasLeft(p);
		}

		/**
			Return true if p is root of tree, false otherwise
		*/
		bool isRoot(TBTPosition *p)
		{
			return p->getIndex() == 1;
		}

		/**
			Add new position
		*/
		TBTPosition* add (T element)
		{
			if(size + 1 >= current_capacity)  //expand 
			{
				current_capacity = size * 2; 
				tree.reserve(current_capacity);
				tree.insert(tree.end(), current_capacity / 2, (TBTPosition*) NULL);
			}
			u32 i = size + 1;
			TBTPosition* new_p = New<TBTPosition, T, u32>(element, i);
			tree[i] = new_p;
	//		Log("added @ %d", i);
			size ++;
			return new_p;
		}
		
		/**
			Remove last node and return the corresponding element
		*/
		T remove () 
		{
			ASSERT(!isEmpty(), "albero vuoto");
			T tmp = tree[size]->getElement();
			Delete(tree[size]);
			tree[size] = 0;
			size --;
			return tmp;
		}
		
		/**
			Return depth of tree
		*/
		u32 depth ()
		{
			//log(base 2)(N+1) N=numero nodi
			 static const double base2 = std::log(2.0);
			 return (u32) (std::log(size - 1) / base2);
		}

		string toString ()
		{
			string ret = "";
			for(u32 i = 1; i <= size; i++)
			{
				T e = tree[i]->getElement();
				ret += format("%d ", e);
			}
			return ret;
		}

		/**
			Mirror of tree
		*/
		void mirror()
		{
			for(u32 i = 1; i < depth(); i++)
			{
				u32 x = (u32) std::pow(2, i);
				u32 y = (u32) std::pow(2, i + 1) - 1;
				for(u32 j = 0; j < (y + 1 - x) / 2; j++)
				{
		//			Log("scambio %d con %d", x+j, y-j);
					TBTPosition* tmp = tree[x+j];
					
					tree[x+j] = tree[y-j];
					tree[x+j]->setIndex(x+j);

					tree[y-j] = tmp;
					tree[y-j]->setIndex(y-j);
				}
			}
			
		}
		
		
};
		

#endif