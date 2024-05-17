ImageBitmap* ImageBitmap::create(const void* pixelData,
                                 uint32_t width,
                                 uint32_t height,
                                 bool isImageBitmapPremultiplied,
                                 bool isImageBitmapOriginClean) {
  return new ImageBitmap(pixelData, width, height, isImageBitmapPremultiplied,
                         isImageBitmapOriginClean);
}
