void FormatConverter::Convert(WebGLImageConversion::DataFormat src_format,
                              WebGLImageConversion::DataFormat dst_format,
                              WebGLImageConversion::AlphaOp alpha_op) {
#define FORMATCONVERTER_CASE_SRCFORMAT(SrcFormat) \
  case SrcFormat:                                 \
    return Convert<SrcFormat>(dst_format, alpha_op);

  switch (src_format) {
    FORMATCONVERTER_CASE_SRCFORMAT(WebGLImageConversion::kDataFormatRA8)
    FORMATCONVERTER_CASE_SRCFORMAT(WebGLImageConversion::kDataFormatRA32F)
    FORMATCONVERTER_CASE_SRCFORMAT(WebGLImageConversion::kDataFormatRGBA8)
    FORMATCONVERTER_CASE_SRCFORMAT(WebGLImageConversion::kDataFormatARGB8)
    FORMATCONVERTER_CASE_SRCFORMAT(WebGLImageConversion::kDataFormatABGR8)
    FORMATCONVERTER_CASE_SRCFORMAT(WebGLImageConversion::kDataFormatAR8)
    FORMATCONVERTER_CASE_SRCFORMAT(WebGLImageConversion::kDataFormatBGRA8)
    FORMATCONVERTER_CASE_SRCFORMAT(WebGLImageConversion::kDataFormatRGBA5551)
    FORMATCONVERTER_CASE_SRCFORMAT(WebGLImageConversion::kDataFormatRGBA4444)
    FORMATCONVERTER_CASE_SRCFORMAT(WebGLImageConversion::kDataFormatRGBA32F)
    FORMATCONVERTER_CASE_SRCFORMAT(
        WebGLImageConversion::kDataFormatRGBA2_10_10_10)
    default:
      NOTREACHED();
  }
#undef FORMATCONVERTER_CASE_SRCFORMAT
}
