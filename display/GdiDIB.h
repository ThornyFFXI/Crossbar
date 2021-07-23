#ifndef __ASHITA_GdiDIB_H_INCLUDED__
#define __ASHITA_GdiDIB_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#include "../Defines.h"
#pragma comment(lib, "C:/Ashita 4/plugins/sdk/d3d8/lib/d3dx8.lib")

class GdiDIB
{
private:
	HBITMAP pDIBSection;
	void* pPixels;
	Gdiplus::Bitmap* pBitmap;
	IDirect3DDevice8* pDevice;
	Gdiplus::Graphics* pGraphics;
	int mWidth;
	int mHeight;

public:
	GdiDIB(IDirect3DDevice8* pDevice, int width, int height);
	~GdiDIB();

	void ApplyToPrimitiveObject(IPrimitiveObject* pPrimitiveObject);
	void ApplySectionToPrimitive(IPrimitiveObject* pPrimitiveObject, int offsetX, int offsetY, int width, int height);
	void ClearRegion(int offsetX, int offsetY, int width, int height);
	void ClearRegion(int offsetX, int offsetY, int width, int height, Gdiplus::Color color);
	Gdiplus::Bitmap* GetBitmap();
	Gdiplus::Graphics* GetGraphics();
	void GetRegionPixels(uint8_t* buffer, int offsetX, int offsetY, int width, int height);
};
#endif