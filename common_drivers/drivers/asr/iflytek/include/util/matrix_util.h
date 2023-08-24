#pragma once

#include "util/isr_defines.h"
namespace matrix_util
{

	template<typename T>
	inline void my_aligned_malloc(T **pMem, size_t nCount, size_t align_size = 64)
	{
		*pMem = (T*)_aligned_malloc(sizeof(T)*nCount, align_size);
		memset(*pMem, 0, nCount*sizeof(T));
	}

	template<typename T>
	inline void my_aligned_free(T **pMem)
	{
		_aligned_free(*pMem);
		*pMem = NULL;
	}

	template<typename T>
	inline void my_aligned_realloc(T **pMem, size_t nCount, int align_size = 64)
	{
		*pMem = (T*)_aligned_realloc(*pMem, sizeof(T)*nCount, align_size);
		memset(*pMem, 0, nCount*sizeof(T));
	}
	template<typename T>
	T GetAlignSize(T nInput, int nAlignUnit= 64)
	{
		if(nInput % nAlignUnit)
		{
			nInput = nInput + nAlignUnit - (nInput%nAlignUnit);
		}
		return nInput;
	}
	template<typename T>
	inline void TransposeMatrix(const T* p_src_mat, int Width, int Height,T *p_dst_mat,int WidthDst,int HeightDst)
	{
		memset(p_dst_mat,0,sizeof(T)* WidthDst * HeightDst);
		for(int i = 0; i < Width; i++)
		{
			for(int j = 0; j < Height; j++)
			{
				p_dst_mat[i*WidthDst + j] = p_src_mat[j*Width + i];
			}
		}
	}
	template<typename T>
	inline void alignMatrix(const T* p_src_mat, int Width, int Height,T *p_dst_mat,int WidthDst,int HeightDst)
	{
		memset(p_dst_mat,0,sizeof(T)* WidthDst * HeightDst);
		for(int i = 0; i < Height; i++)
		{
			for(int j = 0; j < Width; j++)
			{
				p_dst_mat[i*WidthDst + j] = p_src_mat[i*Width + j];
			}
		}
	}
	
	template<typename T>
	inline void matrixMul(T *C, const T *A, const T *B, unsigned int hA, unsigned int wA, unsigned int wB)
	{
		for (unsigned int i = 0; i < hA; ++i)
		{
			for (unsigned int j = 0; j < wB; ++j)
			{
				T sum = 0;
				for (unsigned int k = 0; k < wA; ++k)
				{
					T a = A[i * wA + k];
					T b = B[k * wB + j];
					sum += a * b;
				}
				C[i * wB + j] = (T)sum;
			}
		}
	}
	
};