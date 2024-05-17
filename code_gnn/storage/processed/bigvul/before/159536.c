void WebGLImageConversion::PackPixels(const uint8_t* source_data,
                                      DataFormat source_data_format,
                                      unsigned pixels_per_row,
                                      uint8_t* destination_data) {
  switch (source_data_format) {
    case kDataFormatRA8: {
      typedef typename DataTypeForFormat<
          WebGLImageConversion::kDataFormatRGBA8>::Type SrcType;
      const SrcType* src_row_start = static_cast<const SrcType*>(source_data);
      Pack<WebGLImageConversion::kDataFormatRA8,
           WebGLImageConversion::kAlphaDoUnmultiply>(
          src_row_start, destination_data, pixels_per_row);
    } break;
    case kDataFormatR8: {
      typedef typename DataTypeForFormat<
          WebGLImageConversion::kDataFormatRGBA8>::Type SrcType;
      const SrcType* src_row_start = static_cast<const SrcType*>(source_data);
      Pack<WebGLImageConversion::kDataFormatR8,
           WebGLImageConversion::kAlphaDoUnmultiply>(
          src_row_start, destination_data, pixels_per_row);
    } break;
    case kDataFormatRGBA8: {
      typedef typename DataTypeForFormat<
          WebGLImageConversion::kDataFormatRGBA8>::Type SrcType;
      const SrcType* src_row_start = static_cast<const SrcType*>(source_data);
      Pack<WebGLImageConversion::kDataFormatRGBA8,
           WebGLImageConversion::kAlphaDoUnmultiply>(
          src_row_start, destination_data, pixels_per_row);
    } break;
    case kDataFormatRGBA4444: {
      uint16_t* pdst = (uint16_t*)destination_data;
      typedef typename DataTypeForFormat<
          WebGLImageConversion::kDataFormatRGBA8>::Type SrcType;
      const SrcType* src_row_start = static_cast<const SrcType*>(source_data);
      typedef typename DataTypeForFormat<
          WebGLImageConversion::kDataFormatRGBA4444>::Type DstType;
      DstType* dst_row_start = static_cast<DstType*>(pdst);
      Pack<WebGLImageConversion::kDataFormatRGBA4444,
           WebGLImageConversion::kAlphaDoNothing>(src_row_start, dst_row_start,
                                                  pixels_per_row);
    } break;
    case kDataFormatRGBA5551: {
      uint16_t* pdst = (uint16_t*)destination_data;
      typedef typename DataTypeForFormat<
          WebGLImageConversion::kDataFormatRGBA8>::Type SrcType;
      const SrcType* src_row_start = static_cast<const SrcType*>(source_data);
      typedef typename DataTypeForFormat<
          WebGLImageConversion::kDataFormatRGBA5551>::Type DstType;
      DstType* dst_row_start = static_cast<DstType*>(pdst);
      Pack<WebGLImageConversion::kDataFormatRGBA5551,
           WebGLImageConversion::kAlphaDoNothing>(src_row_start, dst_row_start,
                                                  pixels_per_row);
    } break;
    case kDataFormatRGB565: {
      uint16_t* pdst = (uint16_t*)destination_data;
      typedef typename DataTypeForFormat<
          WebGLImageConversion::kDataFormatRGBA8>::Type SrcType;
      const SrcType* src_row_start = static_cast<const SrcType*>(source_data);
      typedef typename DataTypeForFormat<
          WebGLImageConversion::kDataFormatRGB565>::Type DstType;
      DstType* dst_row_start = static_cast<DstType*>(pdst);
      Pack<WebGLImageConversion::kDataFormatRGB565,
           WebGLImageConversion::kAlphaDoNothing>(src_row_start, dst_row_start,
                                                  pixels_per_row);
    } break;
    default:
      break;
  }
}
