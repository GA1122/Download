void FormatConverter::Convert(WebGLImageConversion::DataFormat dst_format,
                              WebGLImageConversion::AlphaOp alpha_op) {
#define FORMATCONVERTER_CASE_DSTFORMAT(DstFormat) \
  case DstFormat:                                 \
    return Convert<SrcFormat, DstFormat>(alpha_op);

  switch (dst_format) {
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatR8)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatR16F)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatR32F)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatA8)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatA16F)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatA32F)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRA8)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRA16F)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRA32F)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRGB8)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRGB565)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRGB16F)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRGB32F)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRGBA8)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRGBA5551)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRGBA4444)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRGBA16F)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRGBA32F)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRGBA8_S)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRGBA16)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRGBA16_S)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRGBA32)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRGBA32_S)
    FORMATCONVERTER_CASE_DSTFORMAT(
        WebGLImageConversion::kDataFormatRGBA2_10_10_10)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRG8)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRG16F)
    FORMATCONVERTER_CASE_DSTFORMAT(WebGLImageConversion::kDataFormatRG32F)
    default:
      NOTREACHED();
  }

#undef FORMATCONVERTER_CASE_DSTFORMAT
}
