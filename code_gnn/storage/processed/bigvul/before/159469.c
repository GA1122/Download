bool HasAlpha(int format) {
  return format == WebGLImageConversion::kDataFormatA8 ||
         format == WebGLImageConversion::kDataFormatA16F ||
         format == WebGLImageConversion::kDataFormatA32F ||
         format == WebGLImageConversion::kDataFormatRA8 ||
         format == WebGLImageConversion::kDataFormatAR8 ||
         format == WebGLImageConversion::kDataFormatRA16F ||
         format == WebGLImageConversion::kDataFormatRA32F ||
         format == WebGLImageConversion::kDataFormatRGBA8 ||
         format == WebGLImageConversion::kDataFormatBGRA8 ||
         format == WebGLImageConversion::kDataFormatARGB8 ||
         format == WebGLImageConversion::kDataFormatABGR8 ||
         format == WebGLImageConversion::kDataFormatRGBA16F ||
         format == WebGLImageConversion::kDataFormatRGBA32F ||
         format == WebGLImageConversion::kDataFormatRGBA4444 ||
         format == WebGLImageConversion::kDataFormatRGBA5551 ||
         format == WebGLImageConversion::kDataFormatRGBA8_S ||
         format == WebGLImageConversion::kDataFormatRGBA16 ||
         format == WebGLImageConversion::kDataFormatRGBA16_S ||
         format == WebGLImageConversion::kDataFormatRGBA32 ||
         format == WebGLImageConversion::kDataFormatRGBA32_S ||
         format == WebGLImageConversion::kDataFormatRGBA2_10_10_10;
}
