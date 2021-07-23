#include "GdiItem.h"


GdiItem::GdiItem(uint8_t* bitmap, uint32_t size)
{
    int headerSize = 14 + Read32(bitmap, 14);
    int width      = Read32(bitmap, 18);
    int height     = Read32(bitmap, 22);
    int stride      = width * 4;


    BITMAPV4HEADER bmp   = {sizeof(BITMAPV4HEADER)};
    bmp.bV4Width         = width;
    bmp.bV4Height        = height;
    bmp.bV4Planes        = 1;
    bmp.bV4BitCount      = 32;
    bmp.bV4V4Compression = BI_BITFIELDS;
    bmp.bV4RedMask       = 0x00FF0000;
    bmp.bV4GreenMask     = 0x0000FF00;
    bmp.bV4BlueMask      = 0x000000FF;
    bmp.bV4AlphaMask     = 0xFF000000;

    RGBQUAD colors[256];
    for (int x = 0; x < 256; x++)
    {
        colors[x] = *(((RGBQUAD*)(bitmap + 54)) + x);
        if (colors[x].rgbReserved)
            colors[x].rgbReserved = 255;
    }

    this->pDIBSection = ::CreateDIBSection(nullptr, (BITMAPINFO*)&bmp, DIB_RGB_COLORS, &pPixels, nullptr, 0);
    this->pBitmap     = new Gdiplus::Bitmap(width, height, stride, PixelFormat32bppARGB, (BYTE*)this->pPixels);

    uint8_t* offset = bitmap + headerSize + 1024;
    for (int x = 0; x < height; x++)
    {
        uint8_t* pixels = ((uint8_t*)pPixels + ((height - x - 1) * stride));
        for (int y = 0; y < width; y++)
        {
            RGBQUAD color = colors[*offset];
            memcpy(pixels, &color, 4);
            offset++;
            pixels += 4;
        }
    }
}
GdiItem ::~GdiItem()
{
    delete pBitmap;
    DeleteObject(pDIBSection);
}

Gdiplus::Image* GdiItem::GetImage()

{
    return pBitmap;
}