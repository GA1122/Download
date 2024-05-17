void reference_idct_2d(double input[64], int16_t output[64]) {
 double out[64], out2[64];
 for (int i = 0; i < 8; ++i) {
 double temp_in[8], temp_out[8];
 for (int j = 0; j < 8; ++j)
      temp_in[j] = input[j + i*8];
    reference_idct_1d(temp_in, temp_out);
 for (int j = 0; j < 8; ++j)
      out[j + i*8] = temp_out[j];
 }
 for (int i = 0; i < 8; ++i) {
 double temp_in[8], temp_out[8];
 for (int j = 0; j < 8; ++j)
      temp_in[j] = out[j*8 + i];
    reference_idct_1d(temp_in, temp_out);
 for (int j = 0; j < 8; ++j)
      out2[j*8 + i] = temp_out[j];
 }
 for (int i = 0; i < 64; ++i)
    output[i] = round(out2[i]/32);
}
