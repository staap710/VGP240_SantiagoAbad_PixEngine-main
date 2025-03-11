#include "Texture.h"


namespace
{
#pragma pack(push, 1)
    struct BitmapFileHeader
    {
        uint16_t type;          // magic identifier
        uint32_t size;          // file size in bytes
        uint16_t reserved1;
        uint16_t reserved2;
        uint32_t offset;        // byte offset to image data
    };

    struct BitmapInfoHeader
    {
        uint32_t size;                  // header size in bytes
        int width, height;              // width and height of the image
        uint16_t planes;                // number of color planes
        uint16_t bits;                  // bits per pixel
        uint32_t compression;           // compression type
        uint32_t imageSize;             // image size in bytes
        int xResolution, yResolution;   // pixels per meter
        uint32_t numColors;             // number of colors
        uint32_t importantColors;       // important colors
    };

    uint32_t MakeStringAligned(uint32_t rowStride, uint32_t alignStride)
    {
        uint32_t newStride = rowStride;
        while (newStride % alignStride != 0)
        {
            newStride++;
        }

        return newStride;
    }
#pragma pack(pop)

    X::Color GetBilinearFilterPixelColor(const Texture& tex, float u, float v)
    {
        // Step 1. Convert UV Coordinates to Texel Coordinates:
        float uTex = u * static_cast<float>(tex.GetWidth());
        float vTex = v * static_cast<float>(tex.GetHeight());

        // Step 2. Convert the floats to ints to get the pixel indices:
        int uTexInt = static_cast<int>(uTex);
        int vTexInt = static_cast<int>(vTex);

        // Step 3. Get the float remainder as a ratio:
        float uRatio = uTex - static_cast<float>(uTexInt);
        float vRatio = vTex - static_cast<float>(vTexInt);

        // Step 4. Get the opposite ratios:
        float uOpposite = 1.0f - uRatio;
        float vOpposite = 1.0f - vRatio;

        // Step 5. Get all neighboring pixel colors:
        X::Color a = tex.GetPixel(uTexInt, vTexInt) * uOpposite;
        X::Color b = tex.GetPixel(uTexInt + 1, vTexInt) * uRatio;
        X::Color c = tex.GetPixel(uTexInt, vTexInt + 1) * uOpposite;
        X::Color d = tex.GetPixel(uTexInt + 1, vTexInt + 1) * uRatio;

        // Step 6. Calculate the final color:
        return (a + b) * vOpposite
            + (c + d) * vRatio;
    }
}

void Texture::Load(const std::string& fileName)
{
    mFileName = fileName;

    FILE* file = nullptr;
    fopen_s(&file, fileName.c_str(), "rb");

    if (file == nullptr)
    {
        char buffer[128];
        sprintf_s(buffer, "Can't open texture file %s", fileName.c_str());
        MessageBoxA(nullptr, buffer, "Texture Error", MB_OK | MB_ICONEXCLAMATION);
        return;
    }

    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;
    fread(&fileHeader, sizeof(fileHeader), 1, file);
    fread(&infoHeader, sizeof(infoHeader), 1, file);

    if (infoHeader.bits != 24)
    {
        fclose(file);
        MessageBoxA(nullptr, "File not 24 bit aligned!", "Texture Error", MB_OK | MB_ICONEXCLAMATION);
        return;
    }

    mWidth = infoHeader.width;
    mHeight = infoHeader.height;
    mPixels = std::make_unique<X::Color[]>(mWidth * mHeight);

    fseek(file, fileHeader.offset, SEEK_SET);

    uint32_t rowStride = mWidth * infoHeader.bits / 8;
    uint32_t paddedStride = MakeStringAligned(rowStride, 4);
    std::vector<uint8_t> paddedBytes(paddedStride - rowStride);

    for (int h = 0; h < mHeight; ++h)
    {
        for (int w = 0; w < mWidth; ++w)
        {
            uint8_t r, g, b;
            fread(&b, sizeof(uint8_t), 1, file);
            fread(&g, sizeof(uint8_t), 1, file);
            fread(&r, sizeof(uint8_t), 1, file);

            uint32_t index = w + ((mHeight - h - 1) * mWidth);
            mPixels[index] = { r / 255.0f, g / 255.0f, b / 255.0f, 1.0f };
        }
        fread((char*)paddedBytes.data(), paddedBytes.size(), 1, file);
    }
    fclose(file);
}

const std::string& Texture::GetFileName() const
{
    return mFileName;
}

int Texture::GetWidth() const
{
    return mWidth;
}

int Texture::GetHeight() const
{
    return mHeight;
}

X::Color Texture::GetPixel(float u, float v, bool filter, AddressMode addressMode) const
{
    switch (addressMode)
    {
    case AddressMode::Border:
    {
        if (u > 1.0f || u < 0.0f || v > 1.0f || v < 0.0f)
        {
            return X::Colors::HotPink;
        }
    }
    break;
    case AddressMode::Clamp:
    {
        u = std::clamp(u, 0.0f, 1.0f);
        v = std::clamp(v, 0.0f, 1.0f);
    }
    break;
    case AddressMode::Wrap:
    {
        while (u > 1.0f) { u -= 1.0f; }
        while (u < 0.0f) { u += 1.0f; }
        while (v > 1.0f) { v -= 1.0f; }
        while (v < 0.0f) { v += 1.0f; }
    }
    break;
    case AddressMode::Mirror:
    {
        while (u > 2.0f) { u -= 2.0f; }
        while (u < 0.0f) { u += 2.0f; }
        u = (u > 1.0f) ? 2.0f - u : u;

        while (v > 2.0f) { v -= 2.0f; }
        while (v < 0.0f) { v += 2.0f; }
        v = (v > 1.0f) ? 2.0f - v : v;
    }
    break;
    default:
        break;
    }

    if (filter)
    {
        return GetBilinearFilterPixelColor(*this, u, v);
    }

    int uIndex = static_cast<int>(u * (mWidth - 1));
    int vIndex = static_cast<int>(v * (mHeight - 1));

    return GetPixel(uIndex, vIndex);
}

X::Color Texture::GetPixel(int u, int v) const
{
    u = std::clamp(u, 0, mWidth - 1);
    v = std::clamp(v, 0, mHeight - 1);

    return mPixels[u + (v * mWidth)];
}