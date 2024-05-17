bool JPEGImageDecoder::outputScanlines(ImageFrame& buffer)
{
    JSAMPARRAY samples = m_reader->samples();
    jpeg_decompress_struct* info = m_reader->info();
    int width = isScaled ? m_scaledColumns.size() : info->output_width;

    while (info->output_scanline < info->output_height) {
        int sourceY = info->output_scanline;
         
        if (jpeg_read_scanlines(info, samples, 1) != 1)
            return false;

        int destY = scaledY(sourceY);
        if (destY < 0)
            continue;

#if USE(QCMSLIB)
        if (m_reader->colorTransform() && colorSpace == JCS_RGB)
            qcms_transform_data(m_reader->colorTransform(), *samples, *samples, info->output_width);
#endif

        ImageFrame::PixelData* currentAddress = buffer.getAddr(0, destY);
        for (int x = 0; x < width; ++x) {
            setPixel<colorSpace>(buffer, currentAddress, samples, isScaled ? m_scaledColumns[x] : x);
            ++currentAddress;
        }
    }
    return true;
}
