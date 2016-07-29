/*
	file name: Serializable.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 15/04/13
*/
#pragma once
#ifndef __SERIALIZABLE_H__
#define __SERIALIZABLE_H__

class CWriter;
class CReader;

class ISerializable 
{
	virtual		void		Save			(CWriter *fs)			= 0;	
	virtual		void		Load			(CReader *fs)			= 0;
};

#endif