/*
	file name: Gate.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 11/04/2013
*/
#pragma once
#ifndef __GATE_H__
#define __GATE_H__

#ifndef RANDOM_BIT
#define RANDOM_BIT
#endif 

#include "LinkedTree.h"

#define DECLARE_SETTER_PREFIX(prefix, name, type, val) \
			void prefix##name (type a) { val = a; }

#define DECLARE_GETTER_PREFIX(prefix, name, type, val) \
			type prefix##name () const { return val; }

#define DECLARE_SETTER(name, type, val) DECLARE_SETTER_PREFIX(Set, name, type, val)

#define DECLARE_GETTER(name, type, val) DECLARE_GETTER_PREFIX(Get, name, type, val)

#define DECLARE_GETTER_NOCONST(name, type, val) \
			type Get##name ()   { return val; }

#define DECLARE_SETTER_GETTER(name, type, val) \
			DECLARE_SETTER(name, type, val) \
			DECLARE_GETTER(name, type, val) 


class ABSTRACT IGate
{
	typedef CLinkedBinaryTree<IGate*>::SBTreeNode SelfPos;
	friend class CCircuit;
	public:
		enum EGateType
		{
			eAndGate,
			eOrGate,
			eInputGate,
			eOutputGate
		};
	public:
								IGate				(EGateType type) : m_pos(0), m_type(type), m_negate(false)	{ /*m_left = m_right = m_out = 0;*/ }

		virtual					~IGate				()											{ }
		virtual		void		Destroy				()											= 0;
		
	
	
	//	DECLARE_SETTER_GETTER				(Left, IGate*, m_left)
	//	DECLARE_SETTER_GETTER				(Right, IGate*, m_right)
	//	DECLARE_SETTER_GETTER				(Out, IGate*, m_out)
		DECLARE_SETTER_GETTER				(Position, SelfPos*, m_pos)
		DECLARE_GETTER						(Type, EGateType, m_type)
		DECLARE_GETTER_PREFIX				(Is, Negate, bool, m_negate)
		DECLARE_SETTER						(Negate, bool, m_negate)

	protected:
	//	IGate*				m_left;
	//	IGate*				m_right;
	//	IGate*				m_out;
		SelfPos*			m_pos;
		EGateType			m_type;
		bool				m_negate;
};

const int SharesGroup = 2;
const int MaxShares = 3;

template 
<
	class ShareType
>
class ABSTRACT ISharesHolder
{
	friend class CCircuit;
	public:
//		typedef typename detail::STypeHelper<ShareType>::ptype	return_type;
		typedef ShareType											CShareType;
		typedef std::pair<ShareType*, ShareType*>					SharePair; // first: perm :: second: share
		typedef ISharesHolder<ShareType>							Self;

									ISharesHolder		()
#ifdef RANDOM_BIT
																	: m_inverted(false)
#endif
		{
			m_result = SharePair(0, 0);
			for (int i = 0; i < SharesGroup; i++)
			{
				for (int j = 0; j < MaxShares; j++)
				{
					m_elements[i][j].first = NULL;
					m_elements[i][j].second = NULL;
				}
			}
		}
		virtual	~ISharesHolder(){ }

		virtual	void Destroy()
		{  
			for (int i = 0; i < SharesGroup; i++)
			{
				for (int j = 0; j < MaxShares; j++)
				{
			//		Delete(m_elements[i][j].first);
					Delete(m_elements[i][j].second);
				}
			}
		}

		virtual		SharePair**		operator()			()									{ return (SharePair**) m_elements; }
		virtual		void			operator()			(int i, int j, SharePair e)			{ m_elements[i][j] = e; }

		virtual		SharePair&		operator()			(int i, int j)						{ return m_elements[i][j]; }


		DECLARE_SETTER				(Result, SharePair, m_result)
		DECLARE_GETTER_NOCONST		(Result, SharePair&, m_result)
#ifdef RANDOM_BIT
		DECLARE_GETTER_PREFIX		(Is, Inverted, bool, m_inverted)
		DECLARE_SETTER				(Inverted, bool, m_inverted)
#endif
		
	protected:
		SharePair m_elements[SharesGroup][MaxShares];
		SharePair m_result;
#ifdef RANDOM_BIT
		bool m_inverted;
#endif
};

template <class T>
class CAndGate : public IGate, public ISharesHolder<T>
{
	private:
		typedef IGate					base0;
		typedef ISharesHolder<T>		base1;
	public:
				CAndGate()	: base0(EGateType::eAndGate), base1(){ }
		virtual	~CAndGate(){ }
		virtual	void Destroy()
		{
			this->base1::Destroy();
		}
};

template <class T>
class COrGate : public IGate, public ISharesHolder<T>
{
	private:
		typedef IGate					base0;
		typedef ISharesHolder<T>		base1;
	public:
								COrGate				() : base0(EGateType::eOrGate), base1()		{ }
		virtual					~COrGate			()											{ }
		virtual		void		Destroy				()
		{
			this->base1::Destroy();
		}
};

//template <class T>
class CInputGate : public IGate // , public ISharesHolder<T>
{
	private:
		typedef IGate					base0;
	//	typedef ISharesHolder<T>		base1;
	public:
				CInputGate(u16 port_num = 0, bool in = false) : base0(EGateType::eInputGate), /*base1(),*/ m_in(in), m_port_num(port_num) { }
		virtual	~CInputGate(){  }
/*		virtual		return_type execute				()
		{
			return m_value;
		}*/
		DECLARE_SETTER(Input, bool, m_in)
		DECLARE_SETTER_GETTER(PortNumber, u16, m_port_num)

		bool GetInput()
		{
			if (m_negate)
				return !m_in;
			return m_in;
		}

		virtual	void Destroy()
		{
		}

	private:
//		return_type m_value;
		u16	m_port_num;
		bool m_in;
};
#if 0
template <class T>
class COutputGate : public IGate, public ISharesHolder<T, 1, 2>
{
	private:
		typedef IGate					base0;
		typedef ISharesHolder<T>		base1;
	public:
								COutputGate			() : base0(EGateType::eOutputGate)					{ }
		virtual					~COutputGate		()													{ }

		
};

struct SGateCloner
{
	template <class T> static T* Clone(T* obj)
	{
		T* clone = New<T>();			
		for (int i = 0; i < T::SharesGroup; i++)
		{
			for (int j = 0; j < T::MaxShares; j++)
			{
				T::CShareType f = obj(i, j).first;
				T::CShareType s = obj(i, j).second;
				clone(i, j, T::SharePair(T::CShareType::Clone(f), T::CShareType::Clone(s)));
			}
		}
		
	}
};

#endif

#endif