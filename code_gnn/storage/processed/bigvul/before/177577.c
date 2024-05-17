void reference2_16x16_idct_2d(double *input, double *output) {
 double x;
 for (int l = 0; l < 16; ++l) {
 for (int k = 0; k < 16; ++k) {
 double s = 0;
 for (int i = 0; i < 16; ++i) {
 for (int j = 0; j < 16; ++j) {
          x = cos(PI * j * (l + 0.5) / 16.0) *
              cos(PI * i * (k + 0.5) / 16.0) *
              input[i * 16 + j] / 256;
 if (i != 0)
            x *= sqrt(2.0);
 if (j != 0)
            x *= sqrt(2.0);
          s += x;
 }
 }
      output[k*16+l] = s;
 }
 }
}
