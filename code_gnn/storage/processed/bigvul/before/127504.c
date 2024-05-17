void setPixel(ImageFrame& buffer, ImageFrame::PixelData* currentAddress, JSAMPARRAY samples, int column)
{
    JSAMPLE* jsample = *samples + column * (colorSpace == JCS_RGB ? 3 : 4);

    switch (colorSpace) {
    case JCS_RGB:
        buffer.setRGBA(currentAddress, jsample[0], jsample[1], jsample[2], 0xFF);
        break;
    case JCS_CMYK:
        unsigned k = jsample[3];
        buffer.setRGBA(currentAddress, jsample[0] * k / 255, jsample[1] * k / 255, jsample[2] * k / 255, 0xFF);
        break;
    }
}
