#include "stdafx.h"
#include "V2PC.h"
#include "MainForm.h"
#include "windows.h"
#include <utility>
#include "RasterImage.h"

namespace V2PC
{
	using namespace System::Drawing;
	using namespace System::Drawing::Imaging;
	Bitmap^ RasterToBitmap(CRasterImage* original, u32 expand = 16)
	{

		CRasterImage* img = (expand) ? original->ExpandGrid(expand) : original;	
		PixelFormat format = original->GetFormat() == CRasterImage::ePngType ? PixelFormat::Format32bppArgb : PixelFormat::Format24bppRgb;
		const u32 mul = original->GetFormat() == CRasterImage::ePngType ? 4 : 3; 
		Bitmap^ bmp = gcnew Bitmap(img->getWidth(), img->getHeight(), format); // Format24bppRgb
	//	ColorPalette^ ncp = bmp->Palette;
	//	for (int i = 0; i < 256; i++)
	//		ncp->Entries[i] = Color::FromArgb(255, i, i, i);
	//	bmp->Palette = ncp;

		System::Drawing::Rectangle BoundsRect(0, 0, bmp->Width, bmp->Height);
		BitmapData^ bmpData = bmp->LockBits(BoundsRect, ImageLockMode::WriteOnly, bmp->PixelFormat);
		IntPtr iptr = bmpData->Scan0;
		BYTE* ptr = (BYTE*) iptr.ToPointer();
		
		for (s32 y = img->getHeight() - 1; y >= 0; y--)
		{
			BYTE* row = ptr + (y * bmpData->Stride);
			for (u32 x = 0; x < img->getWidth(); x++)
			{
				RGBQUAD pixel = img->Get(x, img->getHeight() - y - 1);
				row[x * mul] = pixel.rgbRed;  
				row[x * mul + 1] = pixel.rgbGreen;  
				row[x * mul + 2] = pixel.rgbBlue;  
				if (mul == 4)
					row[x * mul + 3] = pixel.rgbReserved;
			}
		}

		bmp->UnlockBits(bmpData);
		Delete(img);
		return bmp;
	}

	
	

};
