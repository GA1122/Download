bool JPEGImageDecoder::outputScanlines()
{
    if (m_frameBufferCache.isEmpty())
        return false;

    ImageFrame& buffer = m_frameBufferCache[0];
    if (buffer.status() == ImageFrame::FrameEmpty) {
        if (!buffer.setSize(scaledSize().width(), scaledSize().height()))
            return setFailed();
        buffer.setStatus(ImageFrame::FramePartial);
        buffer.setHasAlpha(true);

        buffer.setOriginalFrameRect(IntRect(IntPoint(), size()));
    }

    jpeg_decompress_struct* info = m_reader->info();

#if defined(TURBO_JPEG_RGB_SWIZZLE)
    if (!m_scaled && turboSwizzled(info->out_color_space)) {
        while (info->output_scanline < info->output_height) {
            unsigned char* row = reinterpret_cast<unsigned char*>(buffer.getAddr(0, info->output_scanline));
            if (jpeg_read_scanlines(info, &row, 1) != 1)
                return false;
#if USE(QCMSLIB)
            if (qcms_transform* transform = m_reader->colorTransform())
                qcms_transform_data_type(transform, row, row, info->output_width, rgbOutputColorSpace() == JCS_EXT_BGRA ? QCMS_OUTPUT_BGRX : QCMS_OUTPUT_RGBX);
#endif
         }
         return true;
     }
#endif

    switch (info->out_color_space) {
    case JCS_RGB:
        return outputScanlines<JCS_RGB>(buffer);
    case JCS_CMYK:
        return outputScanlines<JCS_CMYK>(buffer);
    default:
        ASSERT_NOT_REACHED();
    }

    return setFailed();
}
