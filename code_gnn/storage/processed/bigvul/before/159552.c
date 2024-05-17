void WebGLImageConversion::UnpackPixels(const uint16_t* source_data,
                                        DataFormat source_data_format,
                                        unsigned pixels_per_row,
                                        uint8_t* destination_data) {
  switch (source_data_format) {
    case kDataFormatRGBA4444: {
      typedef typename DataTypeForFormat<
          WebGLImageConversion::kDataFormatRGBA4444>::Type SrcType;
      const SrcType* src_row_start = static_cast<const SrcType*>(source_data);
      Unpack<WebGLImageConversion::kDataFormatRGBA4444>(
          src_row_start, destination_data, pixels_per_row);
    } break;
    case kDataFormatRGBA5551: {
      typedef typename DataTypeForFormat<
          WebGLImageConversion::kDataFormatRGBA5551>::Type SrcType;
      const SrcType* src_row_start = static_cast<const SrcType*>(source_data);
      Unpack<WebGLImageConversion::kDataFormatRGBA5551>(
          src_row_start, destination_data, pixels_per_row);
    } break;
    case kDataFormatBGRA8: {
      const uint8_t* psrc = (const uint8_t*)source_data;
      typedef typename DataTypeForFormat<
          WebGLImageConversion::kDataFormatBGRA8>::Type SrcType;
      const SrcType* src_row_start = static_cast<const SrcType*>(psrc);
      Unpack<WebGLImageConversion::kDataFormatBGRA8>(
          src_row_start, destination_data, pixels_per_row);
    } break;
    default:
      break;
  }
}
