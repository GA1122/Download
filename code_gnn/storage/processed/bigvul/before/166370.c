bool GLES2Util::ComputeImagePaddedRowSize(int width,
                                          int format,
                                          int type,
                                          int alignment,
                                          uint32_t* padded_row_size) {
  uint32_t bytes_per_group = ComputeImageGroupSize(format, type);
  return ComputeImageRowSizeHelper(
      width, bytes_per_group, alignment, nullptr, padded_row_size, nullptr);
}
