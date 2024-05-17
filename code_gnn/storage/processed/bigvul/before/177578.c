void reference_16x16_dct_2d(int16_t input[256], double output[256]) {
 for (int i = 0; i < 16; ++i) {
 double temp_in[16], temp_out[16];
 for (int j = 0; j < 16; ++j)
      temp_in[j] = input[j * 16 + i];
    butterfly_16x16_dct_1d(temp_in, temp_out);
 for (int j = 0; j < 16; ++j)
      output[j * 16 + i] = temp_out[j];
 }
 for (int i = 0; i < 16; ++i) {
 double temp_in[16], temp_out[16];
 for (int j = 0; j < 16; ++j)
      temp_in[j] = output[j + i * 16];
    butterfly_16x16_dct_1d(temp_in, temp_out);
 for (int j = 0; j < 16; ++j)
      output[j + i * 16] = temp_out[j]/2;

   }
 }
