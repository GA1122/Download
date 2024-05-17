void block2d_average_c(uint8_t *src,
 unsigned int src_stride,
 uint8_t *output_ptr,
 unsigned int output_stride,
 unsigned int output_width,
 unsigned int output_height) {
 unsigned int i, j;
 for (i = 0; i < output_height; ++i) {
 for (j = 0; j < output_width; ++j) {
      output_ptr[j] = (output_ptr[j] + src[i * src_stride + j] + 1) >> 1;
 }
    output_ptr += output_stride;
 }
}
