#include "stdafx.h"
#include "Resource.h"
#include "ResourceLoader.h"
#include <direct.h>
#include "RasterImage.h"
#include <fcntl.h>
#include <io.h>

namespace Win32BMPConversion
{
	PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp) 
	{ 
		BITMAP		bmp; 
		PBITMAPINFO pbmi; 
		WORD		cClrBits;
		// Retrieve the bitmap color format, width, and height. 
		if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) 
			return NULL;

		// Convert the color format to a count of bits. 
		cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
		if (cClrBits == 1) 
			cClrBits = 1; 
		else if (cClrBits <= 4) 
			cClrBits = 4; 
		else if (cClrBits <= 8) 
			cClrBits = 8; 
		else if (cClrBits <= 16) 
			cClrBits = 16; 
		else if (cClrBits <= 24) 
			cClrBits = 24; 
		else
			cClrBits = 32; 

		// Allocate memory for the BITMAPINFO structure. (This structure 
		// contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
		// data structures.) 

		// There is no RGBQUAD array for the 24-bit-per-pixel format.
		if (cClrBits != 24) 
			pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<< cClrBits)); 
		else 
			pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER)); 

		// Initialize the fields in the BITMAPINFO structure. 

		pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
		pbmi->bmiHeader.biWidth = bmp.bmWidth; 
		pbmi->bmiHeader.biHeight = bmp.bmHeight; 
		pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
		pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;

		if (cClrBits < 24) 
			pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

		// If the bitmap is not compressed, set the BI_RGB flag. 
		pbmi->bmiHeader.biCompression = BI_RGB; 

		// Compute the number of bytes in the array of color 
		// indices and store the result in biSizeImage. 
		// For Windows NT, the width must be DWORD aligned unless 
		// the bitmap is RLE compressed. This example shows this. 
		// For Windows 95/98/Me, the width must be WORD aligned unless the 
		// bitmap is RLE compressed.
		pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) / 8* pbmi->bmiHeader.biHeight; 
		// Set biClrImportant to 0, indicating that all of the 
		// device colors are important. 
		pbmi->bmiHeader.biClrImportant = 0; 
		return pbmi; 
	} 

	void ReadBMP(HANDLE hf, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC) 
	{ 
		BITMAPFILEHEADER hdr;       // bitmap file-header 
		PBITMAPINFOHEADER pbih;     // bitmap info-header 
		LPBYTE lpBits;              // memory pointer 
		DWORD dwTotal;              // total count of bytes 
		DWORD cb;                   // incremental count of bytes 
		BYTE *hp;                   // byte pointer 
		DWORD dwTmp; 

		pbih = (PBITMAPINFOHEADER) pbi; 
		lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

		ASSERT(hf != INVALID_HANDLE_VALUE, "errore file");

		if (!lpBits) 
			return; 

		// Retrieve the color table (RGBQUAD array) and the bits 
		// (array of palette indices) from the DIB. 
		if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS)) 
			return;
			
		hdr.bfType = 0x4d42;        // BM 
		// Compute the size of the entire file. 
		hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage); 
		hdr.bfReserved1 = 0; 
		hdr.bfReserved2 = 0; 

		// Compute the offset to the array of color indices. 
		hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof (RGBQUAD); 

	
		// Copy the BITMAPFILEHEADER into the .BMP file. 
		if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp,  NULL)) 
		{
			return; 
		}
		// Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
		if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof (RGBQUAD), (LPDWORD) &dwTmp, ( NULL))) 
			return; 

		// Copy the array of color indices into the .BMP file. 
		dwTotal = cb = pbih->biSizeImage; 
		hp = lpBits; 
		if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL)) 
			return; 
 
		GlobalFree((HGLOBAL)lpBits);
		
	}

}

#if _MSC_VER >= 1300 
#ifndef _delayimp_h
extern "C" IMAGE_DOS_HEADER __ImageBase;
#endif
#endif
     
HMODULE GetCurrentModule()
{
#if _MSC_VER < 1300 
	MEMORY_BASIC_INFORMATION mbi;
	static int dummy;
	VirtualQuery(&dummy, &mbi, sizeof(mbi)); 
	return reinterpret_cast<HMODULE>(mbi.AllocationBase);    
#else 
	return reinterpret_cast<HMODULE>(&__ImageBase);
#endif
}

CRasterImage* LoadDllBitmap(u16 resource_id, s32 size_x, s32 size_y)
{
	HINSTANCE dll = GetCurrentModule();
	HANDLE hBmp = ::LoadImage(dll, MAKEINTRESOURCE(resource_id), IMAGE_BITMAP, 
				size_x, size_y, LR_CREATEDIBSECTION | LR_COLOR | LR_COPYFROMRESOURCE | LR_DEFAULTSIZE);
	CRasterImage::EImageType type = CRasterImage::eBitmapType;
	u16 depth = DEFAULT_BITS_PER_PIXEL;
	CRasterImage* bitmap = New<CRasterImage>(size_x, size_y, depth, type);
	bitmap->Load((HBITMAP) hBmp);

/*
	PBITMAPINFO pBitmapInfo = Win32BMPConversion::CreateBitmapInfoStruct((HBITMAP)hBmp);
	char temp_name[MAX_PATH], current_path[MAX_PATH];
	_getcwd(current_path, sizeof(current_path));
	GetTempFileName(current_path, NULL, 0, temp_name);
	FILE *fp = fopen(temp_name, "wb+");
	HANDLE hf = (HANDLE) _get_osfhandle(_fileno(fp));
	Win32BMPConversion::ReadBMP(hf, pBitmapInfo, (HBITMAP) hBmp, GetDC(NULL));
	fclose(fp);
	fp = fopen(temp_name, "rb");
	
	bitmap = New<CRasterImage>();
	bitmap->Load(fp);
	fclose(fp);
	LocalFree(pBitmapInfo);
	DeleteFile(temp_name);
	return bitmap;
*/
	return bitmap;
}

void LoadGatesImages(SGatesImages* data, s32 size_x, s32 size_y)
{
	data->m_andPort = LoadDllBitmap(IDB_BITMAP1 , size_x, size_y);
	data->m_nandPort = LoadDllBitmap(IDB_BITMAP6, size_x, size_y);
	data->m_orPort = LoadDllBitmap(IDB_BITMAP8, size_x, size_y);
	data->m_norPort = LoadDllBitmap(IDB_BITMAP7, size_x, size_y);
	data->m_leftWire = LoadDllBitmap(IDB_BITMAP4, size_x, size_y);
	data->m_rightWire = LoadDllBitmap(IDB_BITMAP3, size_x, size_y);
	data->m_topWire = LoadDllBitmap	(IDB_BITMAP5, size_x, size_y);
	data->m_Wire = LoadDllBitmap(IDB_BITMAP2, size_x, size_y);
}