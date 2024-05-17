bool GLES2Util::ComputeImageRowSizeHelper(int width,
                                          uint32_t bytes_per_group,
                                          int alignment,
                                          uint32_t* rt_unpadded_row_size,
                                          uint32_t* rt_padded_row_size,
                                          uint32_t* rt_padding) {
  DCHECK(alignment == 1 || alignment == 2 ||
         alignment == 4 || alignment == 8);
  uint32_t unpadded_row_size;
  if (!SafeMultiplyUint32(width, bytes_per_group, &unpadded_row_size)) {
    return false;
  }
  uint32_t residual = unpadded_row_size % alignment;
  uint32_t padding = 0;
  uint32_t padded_row_size = unpadded_row_size;
  if (residual > 0) {
    padding = alignment - residual;
    if (!SafeAddUint32(unpadded_row_size, padding, &padded_row_size)) {
      return false;
    }
  }

  if (rt_unpadded_row_size)
    *rt_unpadded_row_size = unpadded_row_size;
  if (rt_padded_row_size)
    *rt_padded_row_size = padded_row_size;
  if (rt_padding)
    *rt_padding = padding;
  return true;
}
