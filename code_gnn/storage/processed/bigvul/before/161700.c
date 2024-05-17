VaapiVideoDecodeAccelerator::VaapiDecodeSurface::VaapiDecodeSurface(
    int32_t bitstream_id,
    const scoped_refptr<VASurface>& va_surface)
    : bitstream_id_(bitstream_id), va_surface_(va_surface) {}
