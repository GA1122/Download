bool venc_dev::venc_color_align(OMX_BUFFERHEADERTYPE *buffer,
        OMX_U32 width, OMX_U32 height)
{
    OMX_U32 y_stride = VENUS_Y_STRIDE(COLOR_FMT_NV12, width),
            y_scanlines = VENUS_Y_SCANLINES(COLOR_FMT_NV12, height),
            uv_stride = VENUS_UV_STRIDE(COLOR_FMT_NV12, width),
            uv_scanlines = VENUS_UV_SCANLINES(COLOR_FMT_NV12, height),
            src_chroma_offset = width * height;

 if (buffer->nAllocLen >= VENUS_BUFFER_SIZE(COLOR_FMT_NV12, width, height)) {
        OMX_U8* src_buf = buffer->pBuffer, *dst_buf = buffer->pBuffer;
        src_buf += width * height;
        dst_buf += y_stride * y_scanlines;
 for (int line = height / 2 - 1; line >= 0; --line) {
            memmove(dst_buf + line * uv_stride,
                    src_buf + line * width,
                    width);
 }

        dst_buf = src_buf = buffer->pBuffer;
 for (int line = height - 1; line > 0; --line) {
            memmove(dst_buf + line * y_stride,
                    src_buf + line * width,
                    width);
 }
 } else {
        DEBUG_PRINT_ERROR("Failed to align Chroma. from %u to %u : \
 Insufficient bufferLen=%u v/s Required=%u",
 (unsigned int)(width*height), (unsigned int)src_chroma_offset, (unsigned int)buffer->nAllocLen,
                VENUS_BUFFER_SIZE(COLOR_FMT_NV12, width, height));
 return false;
 }

 return true;
}
