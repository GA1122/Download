void reference_dct_2d(int16_t input[64], double output[64]) {
 for (int i = 0; i < 8; ++i) {
 double temp_in[8], temp_out[8];
 for (int j = 0; j < 8; ++j)
      temp_in[j] = input[j*8 + i];
    reference_dct_1d(temp_in, temp_out);
 for (int j = 0; j < 8; ++j)
      output[j*8 + i] = temp_out[j];
 }
 for (int i = 0; i < 8; ++i) {
 double temp_in[8], temp_out[8];
 for (int j = 0; j < 8; ++j)
      temp_in[j] = output[j + i*8];
    reference_dct_1d(temp_in, temp_out);
 for (int j = 0; j < 8; ++j)
      output[j + i*8] = temp_out[j];
 }
 for (int i = 0; i < 64; ++i)
    output[i] *= 2;
}
