 void fdct16x16_ref(const int16_t *in, int16_t *out, int stride, int tx_type) {
  vp9_fdct16x16_c(in, out, stride);
// void fdct16x16_ref(const int16_t *in, tran_low_t *out, int stride,
//                    int  ) {
//   vpx_fdct16x16_c(in, out, stride);
  }