#include "GdiDIB.h"

#define SAVE_TO_HD

#ifdef SAVE_TO_HD

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

    Gdiplus::GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  // Failure

    pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1;  // Failure

    Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

#endif

GdiDIB::GdiDIB(IDirect3DDevice8* pDevice, int width, int height)
    : pDevice(pDevice)
{
    this->mWidth = width;
    this->mHeight = height;


    BITMAPV4HEADER bmp = { sizeof(BITMAPV4HEADER) };
    bmp.bV4Width = width;
    bmp.bV4Height = height;
    bmp.bV4Planes = 1;
    bmp.bV4BitCount = 32;
    bmp.bV4V4Compression = BI_BITFIELDS;
    bmp.bV4RedMask = 0x00FF0000;
    bmp.bV4GreenMask = 0x0000FF00;
    bmp.bV4BlueMask = 0x000000FF;
    bmp.bV4AlphaMask = 0xFF000000;

    this->pDIBSection = ::CreateDIBSection(nullptr, (BITMAPINFO*)&bmp, DIB_RGB_COLORS, &pPixels, nullptr, 0);
    this->pBitmap = new Gdiplus::Bitmap(width, height, width * 4, PixelFormat32bppARGB, (BYTE*)this->pPixels);
    this->pGraphics = new Gdiplus::Graphics(this->pBitmap);
}
GdiDIB::~GdiDIB()
{
    delete pGraphics;
    delete pBitmap;
    DeleteObject(pDIBSection);
}

void GdiDIB::ApplyToPrimitiveObject(IPrimitiveObject* pPrimitiveObject)
{
#ifdef SAVE_TO_HD
    CLSID pngClsid;
    GetEncoderClsid(L"image/png", &pngClsid);
    this->pBitmap->Save(L"C:\\crossbartest.png", &pngClsid, NULL);
#endif

    IDirect3DTexture8* texture = nullptr;
    ::D3DXCreateTexture(pDevice, mWidth, mHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture);

    D3DLOCKED_RECT rect{};
    texture->LockRect(0, &rect, 0, 0);
    {
        const auto srcPixels = (uint8_t*)pPixels;
        const auto dstPixels = (uint8_t*)rect.pBits;
        memcpy(dstPixels, srcPixels, mWidth * mHeight * 4);
    }
    texture->UnlockRect(0);

    pPrimitiveObject->SetTextureFromTexture(texture, mWidth, mHeight);
    pPrimitiveObject->SetWidth(pBitmap->GetWidth());
    pPrimitiveObject->SetHeight(pBitmap->GetHeight());
    texture->Release();
}

void GdiDIB::ApplySectionToPrimitive(IPrimitiveObject* pPrimitiveObject, int offsetX, int offsetY, int width, int height)
{
    if ((offsetX + width) > mWidth)
        return;
    if ((offsetY + height) > mHeight)
        return;

    IDirect3DTexture8* texture = nullptr;
    ::D3DXCreateTexture(pDevice, width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture);

    D3DLOCKED_RECT rect{};
    texture->LockRect(0, &rect, 0, 0);
    {
        GetRegionPixels((uint8_t*)rect.pBits, offsetX, offsetY, width, height);
    }
    texture->UnlockRect(0);

#ifdef SAVE_TO_HD
    void* pPixels;
    BITMAPV4HEADER bmp = { sizeof(BITMAPV4HEADER) };
    bmp.bV4Width = width;
    bmp.bV4Height = height;
    bmp.bV4Planes = 1;
    bmp.bV4BitCount = 32;
    bmp.bV4V4Compression = BI_BITFIELDS;
    bmp.bV4RedMask = 0x00FF0000;
    bmp.bV4GreenMask = 0x0000FF00;
    bmp.bV4BlueMask = 0x000000FF;
    bmp.bV4AlphaMask = 0xFF000000;
    HBITMAP pBmp = ::CreateDIBSection(nullptr, (BITMAPINFO*)&bmp, DIB_RGB_COLORS, &pPixels, nullptr, 0);
    Gdiplus::Bitmap* pRaw = new Gdiplus::Bitmap(width, height, width * 4, PixelFormat32bppARGB, (BYTE*)pPixels);
    GetRegionPixels((uint8_t*)pPixels, offsetX, offsetY, width, height);

    CLSID pngClsid;
    GetEncoderClsid(L"image/png", &pngClsid);
    pRaw->Save(L"C:\\crossbartest.png", &pngClsid, NULL);
    delete pRaw;
    DeleteObject(pBmp);
#endif

    pPrimitiveObject->SetTextureFromTexture(texture, width, height);
    pPrimitiveObject->SetWidth(width);
    pPrimitiveObject->SetHeight(height);
    texture->Release();
}

void GdiDIB::ClearRegion(int offsetX, int offsetY, int width, int height)
{
    int stride = mWidth * 4;
    int pitch = width * 4;
    int scanOffset = (stride * offsetY) + (offsetX * 4);
    uint8_t* dstPixels = (uint8_t*)pPixels + scanOffset;
    for (int y = 0; y < height; y++)
    {
        memset(dstPixels, 0, pitch);
        dstPixels += stride;
    }
}
void GdiDIB::ClearRegion(int offsetX, int offsetY, int width, int height, Gdiplus::Color color)
{
    int bmpLineSize = mWidth * 4;
    int regionLineSize = width * 4;

    uint8_t* lineBuffer = new uint8_t[regionLineSize];
    uint8_t* pBuffer = lineBuffer;
    for (int x = 0; x < width; x++)
    {
        pBuffer[0] = color.GetRed();
        pBuffer[1] = color.GetGreen();
        pBuffer[2] = color.GetBlue();
        pBuffer[3] = color.GetAlpha();
        pBuffer += 4;
    }

    int scanOffset = (mWidth * 4 * offsetY) + (offsetX * 4);
    uint8_t* dstPixels = (uint8_t*)pPixels + scanOffset;
    for (int y = 0; y < height; y++)
    {
        memcpy(dstPixels, lineBuffer, regionLineSize);
        dstPixels += bmpLineSize;
    }
    delete[] lineBuffer;
}

Gdiplus::Bitmap* GdiDIB::GetBitmap()
{
    return pBitmap;
}
Gdiplus::Graphics* GdiDIB::GetGraphics()
{
    return pGraphics;
}

void GdiDIB::GetRegionPixels(uint8_t* buffer, int offsetX, int offsetY, int width, int height)
{
    int stride = mWidth * 4;
    int pitch = width * 4;
    int scanOffset = (stride * offsetY) + (offsetX * 4);
    uint8_t* srcPixels = (uint8_t*)pPixels + scanOffset;
    for (int y = 0; y < height; y++)
    {
        memcpy(buffer, srcPixels, pitch);
        srcPixels += stride;
        buffer += pitch;
    }
}