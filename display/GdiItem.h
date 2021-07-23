#ifndef __ASHITA_GdiItem_H_INCLUDED__
#define __ASHITA_GdiItem_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#include "../Defines.h"
#pragma comment(lib, "C:/Ashita 4/plugins/sdk/d3d8/lib/d3dx8.lib")

class GdiItem
{
private:
    HBITMAP pDIBSection;
    void* pPixels;
    Gdiplus::Bitmap* pBitmap;

public:
    GdiItem(uint8_t* bitmap, uint32_t size);
    ~GdiItem();

    Gdiplus::Image* GetImage();
};
#endif