void reference_idct4x4(const int16_t *input, int16_t *output) {
 const int16_t *ip = input;
 int16_t *op = output;

 for (int i = 0; i < 4; ++i) {
 const int a1 = ip[0] + ip[8];
 const int b1 = ip[0] - ip[8];
 const int temp1 = (ip[4] * sinpi8sqrt2) >> 16;
 const int temp2 = ip[12] + ((ip[12] * cospi8sqrt2minus1) >> 16);
 const int c1 = temp1 - temp2;
 const int temp3 = ip[4] + ((ip[4] * cospi8sqrt2minus1) >> 16);
 const int temp4 = (ip[12] * sinpi8sqrt2) >> 16;
 const int d1 = temp3 + temp4;
    op[0] = a1 + d1;
    op[12] = a1 - d1;
    op[4] = b1 + c1;
    op[8] = b1 - c1;
 ++ip;
 ++op;
 }
  ip = output;
  op = output;
 for (int i = 0; i < 4; ++i) {
 const int a1 = ip[0] + ip[2];
 const int b1 = ip[0] - ip[2];
 const int temp1 = (ip[1] * sinpi8sqrt2) >> 16;
 const int temp2 = ip[3] + ((ip[3] * cospi8sqrt2minus1) >> 16);
 const int c1 = temp1 - temp2;
 const int temp3 = ip[1] + ((ip[1] * cospi8sqrt2minus1) >> 16);
 const int temp4 = (ip[3] * sinpi8sqrt2) >> 16;
 const int d1 = temp3 + temp4;
    op[0] = (a1 + d1 + 4) >> 3;
    op[3] = (a1 - d1 + 4) >> 3;
    op[1] = (b1 + c1 + 4) >> 3;
    op[2] = (b1 - c1 + 4) >> 3;
    ip += 4;
    op += 4;
 }
}
