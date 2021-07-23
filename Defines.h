#ifndef __ASHITA_CrossbarDefines_H_INCLUDED__
#define __ASHITA_CrossbarDefines_H_INCLUDED__
#include "C:\Ashita 4\plugins\sdk\Ashita.h"
#include "thirdparty\rapidxml.hpp"
#include <Windows.h>
#include <atlbase.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")
#include <filesystem>
#include <fstream>

#define RBUFP(p,pos) (((unsigned char*)(p)) + (pos))
#define Read8(p,pos) (*(unsigned char*)RBUFP((p),(pos)))
#define Read16(p,pos) (*(unsigned short*)RBUFP((p),(pos)))
#define Read32(p,pos) (*(unsigned int*)RBUFP((p),(pos)))
#define Read64(p,pos) (*(unsigned long*)RBUFP((p),(pos)))
#define ReadFloat(p,pos) (*(float*)RBUFP((p),(pos)))

#define WBUFP(p,pos) (((unsigned char*)(p)) + (pos))
#define Write8(p,pos) (*(unsigned char*)WBUFP((p),(pos)))
#define Write16(p,pos) (*(unsigned short*)WBUFP((p),(pos)))
#define Write32(p,pos) (*(unsigned int*)WBUFP((p),(pos)))
#define Write64(p,pos) (*(unsigned long*)WBUFP((p),(pos)))
#define WriteFloat(p,pos) (*(float*)WBUFP((p),(pos)))

class Crossbar;
class CrossbarXInput;
class CrossbarDirectInput;
class InputHandler;

enum class MacroButton
{
    DpadUp = 0,
    DpadRight = 1,
    DpadDown = 2,
    DpadLeft = 3,
    ButtonUp = 4,
    ButtonRight = 5,
    ButtonDown = 6,
    ButtonLeft = 7,
    ShoulderLeft = 8,
    ShoulderRight = 9,
    Confirm = 10,
    Cancel = 11
};
enum class MacroMode
{
    LeftTrigger = 0,
    RightTrigger = 1,
    BothTriggersLT = 2,
    BothTriggersRT = 3,
    LeftTriggerDT = 4,
    RightTriggerDT = 5,
    NoTrigger = 6
};

#define SAFE_DELETE(a) if (a != NULL) delete a
#define SN_TO_INT(a,b) if (_stricmp(subNode->name(), a) == 0) b = atoi(subNode->value())
#define E_SN_TO_INT(a,b) else if (_stricmp(subNode->name(), a) == 0) b = atoi(subNode->value())

using namespace rapidxml;

class XmlLoader
{
public:
    static void CreateDirectories(const char* path)
    {
        std::string makeDirectory(path);
        size_t nextDirectory = makeDirectory.find_first_of("\\/");
        nextDirectory = makeDirectory.find_first_of("\\/", nextDirectory + 1);
        while (nextDirectory != std::string::npos)
        {
            std::string currentDirectory = makeDirectory.substr(0, nextDirectory + 1);
            CreateDirectory(currentDirectory.c_str(), NULL);
            nextDirectory = makeDirectory.find_first_of("\\/", nextDirectory + 1);
        }
    }
    static bool Load(char** buffer, xml_document<>** docBuffer, const char* path)
    {
        std::ifstream inputStream = std::ifstream(path, std::ios::in | std::ios::binary | std::ios::ate);
        if (!inputStream.is_open())
        {
            return NULL;
        }

        long size = inputStream.tellg();
        *buffer = new char[size + 1];
        inputStream.seekg(0, std::ios::beg);
        inputStream.read(*buffer, size);
        (*buffer)[size] = '\0';

        *docBuffer = new xml_document<>();
        try
        {
            (*docBuffer)->parse<0>(*buffer);
        }
        catch (const parse_error& e)
        {
            delete* docBuffer;
            delete[] * buffer;
            return false;
        }
        catch (...)
        {
            delete* docBuffer;
            delete[] * buffer;
            return false;
        }

        return true;
    }

    static std::string Escape(const char* input) {
        int len = strlen(input);
        std::string buffer;
        for (size_t pos = 0; pos < len; ++pos) {
            switch (input[pos]) {
            case '&':  buffer.append("&amp;");       break;
            case '\"': buffer.append("&quot;");      break;
            case '\'': buffer.append("&apos;");      break;
            case '<':  buffer.append("&lt;");        break;
            case '>':  buffer.append("&gt;");        break;
            default:   buffer.push_back(input[pos]); break;
            }
        }
        return buffer;
    }
    static bool IsNumber(const char* cString)
    {
        int len = strlen(cString);
        for (int x = 0; x < len; x++)
        {
            if (!isdigit(cString[x]))
            {
                return false;
            }
        }
        return true;
    }

    static Gdiplus::Color ToColor(xml_node<>* baseNode)
    {
        int alpha = 255;
        int red = 255;
        int green = 255;
        int blue = 255;

        for (xml_node<>* subNode = baseNode->first_node(); subNode; subNode = subNode->next_sibling())
        {
            SN_TO_INT("alpha", alpha);
            E_SN_TO_INT("red", red);
            E_SN_TO_INT("green", green);
            E_SN_TO_INT("blue", blue);
        }
        return Gdiplus::Color(alpha, red, green, blue);
    }

    static Gdiplus::Pen* ToPen(xml_node<>* baseNode)
    {
        Gdiplus::Color color(255, 255, 255, 255);
        int width = 2;

        for (xml_node<>* subNode = baseNode->first_node(); subNode; subNode = subNode->next_sibling())
        {
            SN_TO_INT("width", width);
else if (_stricmp(subNode->name(), "color") == 0)
            {
            color = ToColor(subNode);
            }
        }
        return new Gdiplus::Pen(color, width);
    }

    static Gdiplus::Brush* ToBrush(xml_node<>* baseNode)
    {
        return new Gdiplus::SolidBrush(ToColor(baseNode));
    }
};
class GDIFontInfo
{
public:
    int TextHeight;
    int OutlineWidth;
    Gdiplus::StringFormat* pFormat;
    Gdiplus::FontFamily* pFont;
    Gdiplus::Pen* pPen;
    Gdiplus::Brush* pBrush;
    Gdiplus::FontStyle Style;

    GDIFontInfo()
        : pFormat(NULL)
        , pFont(NULL)
        , pPen(NULL)
        , pBrush(NULL)
    {}

    GDIFontInfo(xml_node<>* baseNode)
    {
        pFormat = new Gdiplus::StringFormat();
        pFormat->SetAlignment(Gdiplus::StringAlignmentNear);
        for (xml_node<>* subNode = baseNode->first_node(); subNode; subNode = subNode->next_sibling())
        {
            if (_stricmp(subNode->name(), "family") == 0)
            {
                wchar_t fontBuffer[256];
                swprintf_s(fontBuffer, 256, L"%S", subNode->value());
                pFont = new Gdiplus::FontFamily(fontBuffer);
            }
            else if (_stricmp(subNode->name(), "size") == 0)
            {
                TextHeight = atoi(subNode->value());
            }
            else if (_stricmp(subNode->name(), "flags") == 0)
            {
                int fontStyle = static_cast<int>(Gdiplus::FontStyle::FontStyleRegular);
                std::stringstream stream(subNode->value());
                while (stream.good())
                {
                    std::string format;
                    std::getline(stream, format, ',');
                    if (format.length() < 1)
                        break;

                    if (_stricmp(format.c_str(), "bold") == 0)
                        fontStyle |= static_cast<int>(Gdiplus::FontStyle::FontStyleBold);

                    if (_stricmp(format.c_str(), "italic") == 0)
                        fontStyle |= static_cast<int>(Gdiplus::FontStyle::FontStyleItalic);

                    if (_stricmp(format.c_str(), "underline") == 0)
                        fontStyle |= static_cast<int>(Gdiplus::FontStyle::FontStyleUnderline);

                    if (_stricmp(format.c_str(), "strikeout") == 0)
                        fontStyle |= static_cast<int>(Gdiplus::FontStyle::FontStyleStrikeout);
                }
                Style = static_cast<Gdiplus::FontStyle>(fontStyle);
            }
            else if (_stricmp(subNode->name(), "alignment") == 0)
            {
                if (_stricmp(subNode->value(), "near") == 0)
                    pFormat->SetAlignment(Gdiplus::StringAlignmentNear);
                else if (_stricmp(subNode->value(), "center") == 0)
                    pFormat->SetAlignment(Gdiplus::StringAlignmentCenter);
                else if (_stricmp(subNode->value(), "far") == 0)
                    pFormat->SetAlignment(Gdiplus::StringAlignmentFar);
            }
            else if (_stricmp(subNode->name(), "color") == 0)
            {
                pBrush = XmlLoader::ToBrush(subNode);
            }
            else if (_stricmp(subNode->name(), "outline") == 0)
            {
                pPen = XmlLoader::ToPen(subNode);
            }
        }
    }
    ~GDIFontInfo()
    {
        SAFE_DELETE(pFormat);
        SAFE_DELETE(pFont);
        SAFE_DELETE(pPen);
        SAFE_DELETE(pBrush);
    }
};
class GDITextElementInfo
{
public:
    bool Draw;
    GDIFontInfo* pFont;
    int OffsetX, OffsetY;
    int Width;
    int Height;
    int Z;

    GDITextElementInfo()
    {
        Draw = false;
        pFont = NULL;
    }
    GDITextElementInfo(xml_node<>* baseNode)
        : pFont(NULL)
    {
        xml_attribute<>* attr = baseNode->first_attribute("enabled");
        if ((!attr) || (_stricmp(attr->value(), "true") != 0))
        {
            Draw = false;
        }
        Draw = true;

        attr = baseNode->first_attribute("z");
        if (attr)
            Z = atoi(attr->value());
        else
            Z = 0;

        for (xml_node<>* subNode = baseNode->first_node(); subNode; subNode = subNode->next_sibling())
        {
            if (_stricmp(subNode->name(), "font") == 0)
            {
                pFont = new GDIFontInfo(subNode);
            }
            E_SN_TO_INT("offsetx", OffsetX);
            E_SN_TO_INT("offsety", OffsetY);
            E_SN_TO_INT("width", Width);
            E_SN_TO_INT("height", Height);
        }
    }

    ~GDITextElementInfo()
    {
        SAFE_DELETE(pFont);
    }

    void AddToPath(Gdiplus::GraphicsPath* pPath, const wchar_t* text, Gdiplus::Rect rect)
    {
        pPath->AddString(text, wcslen(text), pFont->pFont, pFont->Style, pFont->TextHeight, rect, pFont->pFormat);
    }
};
#endif