static GLint ComputeNumRowsThatFitInBuffer(uint32_t padded_row_size,
                                           uint32_t unpadded_row_size,
                                           unsigned int size,
                                           GLsizei remaining_rows) {
  DCHECK_GE(unpadded_row_size, 0u);
  if (padded_row_size == 0) {
    return 1;
  }
  GLint num_rows = size / padded_row_size;
  if (num_rows + 1 == remaining_rows &&
      size - num_rows * padded_row_size >= unpadded_row_size) {
    num_rows++;
  }
  return num_rows;
}
