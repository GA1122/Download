static void swizzleBGRAToRGBA(uint32_t* data, const IntSize& size)
{
    int width = size.width();
    int height = size.height();
    for (int y = 0; y < height; ++y) {
        uint32_t* p = data + y * width;
        for (int x = 0; x < width; ++x)
            p[x] = ((p[x] << 16) & 0xff0000) | ((p[x] >> 16) & 0xff) | (p[x] & 0xff00ff00);
    }
}
