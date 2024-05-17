bool GLES2Util::ComputeImageDataSizes(int width,
                                      int height,
                                      int depth,
                                      int format,
                                      int type,
                                      int alignment,
                                      uint32_t* size,
                                      uint32_t* opt_unpadded_row_size,
                                      uint32_t* opt_padded_row_size) {
  PixelStoreParams params;
  params.alignment = alignment;
  return ComputeImageDataSizesES3(
      width, height, depth, format, type, params,
      size, opt_unpadded_row_size, opt_padded_row_size, nullptr, nullptr);
}
