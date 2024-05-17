void SoftG711::DecodeALaw(
 int16_t *out, const uint8_t *in, size_t inSize) {
 while (inSize-- > 0) {
 int32_t x = *in++;

 int32_t ix = x ^ 0x55;
        ix &= 0x7f;

 int32_t iexp = ix >> 4;
 int32_t mant = ix & 0x0f;

 if (iexp > 0) {
            mant += 16;
 }

        mant = (mant << 4) + 8;

 if (iexp > 1) {
            mant = mant << (iexp - 1);
 }

 *out++ = (x > 127) ? mant : -mant;
 }
}
