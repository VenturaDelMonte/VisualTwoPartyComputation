/*
	file name: Matrix.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 27/03/2013
*/

#pragma once
#ifndef __MATRIX_H
#define __MATRIX_H

template <typename T> struct Matrix
{
	public:
		typedef T TYPE;
	private:
		const int rows;
		const int columns;
		T** M;
	public:
		inline Matrix(int r, int c) : rows(r), columns(c)
		{
			ASSERT(rows > 0 && columns > 0, *format("invalid rows(%d) or cols(%d)", rows, columns));
			M = MemHandler.Alloc<T*>(rows);
			for(int i = 0; i < rows; i++)
			{
				M[i] = MemHandler.Alloc<T>(columns); 
			}
			//ZeroMemory(M, rows * columns * sizeof(T));
		}
		virtual ~Matrix()
		{
			for(int i = 0; i < rows; i++)
			{
				ASSERT(M[i], "shit");
				Delete(M[i]);
			}
			Delete(M);
		}
		int getNColumns ()
		{
			return columns;
		}

		int getNRows ()
		{
			return rows;
		}	
		
		int sizeMatrix()
		{
			return rows * columns;
		}

		T* getAt(int i)
		{
			ASSERT(i < rows, *format("wrong pos[%d]", i));
			return M[i];
		}

		T& getAt(int i, int j)
		{
			ASSERT(i < rows && j < columns, *format("wrong pos[%d][%d]", i, j));
			return M[i][j];
		}

		void setAt(int i, int j, T e)
		{
			ASSERT(i < rows && j < columns, *format("wrong pos[%d][%d]", i, j));
			M[i][j] = e;
		}

		T* operator[] (int i)
		{
			ASSERT(i < rows, *format("wrong pos %d", i));
			return	M[i];
		}

		const T* operator[] (int i) const
		{
			ASSERT(i < rows, *format("wrong pos %d", i));
			return	M[i];
		}

		Matrix<T>& operator+ (Matrix m)
		{
			for(int i = 0; i < rows; i++)
			{
				for(int j = 0; j < rows; j++)
				{
					M[i][j] = M[i][j] + m[i][j];
				}
			}
			return *this;
		}
	
};

typedef Matrix<float>	FMatrix;
typedef Matrix<int>		IMatrix;
typedef Matrix<double>	DMatrix;
typedef Matrix<bool>	BMatrix;

#endif