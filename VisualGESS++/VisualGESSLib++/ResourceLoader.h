#pragma once
#ifndef __RESOURCE_LOADER_H__
#define __RESOURCE_LOADER_H__

class CRasterImage;

struct SGatesImages
{
	CRasterImage* m_andPort;
	CRasterImage* m_nandPort;
	CRasterImage* m_orPort;
	CRasterImage* m_norPort;
	CRasterImage* m_rightWire;
	CRasterImage* m_leftWire;
	CRasterImage* m_topWire;
	CRasterImage* m_Wire;
};

void LoadGatesImages(SGatesImages* data, s32 size_x, s32 size_y);
CRasterImage* LoadDllBitmap(u16 resource_id, s32 size_x, s32 size_y);

#endif
