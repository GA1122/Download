const CompressedFormatInfo* GetCompressedFormatInfo(GLenum format) {
  for (size_t i = 0; i < base::size(kCompressedFormatInfoArray); i++) {
    if (kCompressedFormatInfoArray[i].format == format) {
      return &kCompressedFormatInfoArray[i];
    }
  }
  return nullptr;
}
