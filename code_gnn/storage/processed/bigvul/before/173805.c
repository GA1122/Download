void SoftG711::DecodeMLaw(
 int16_t *out, const uint8_t *in, size_t inSize) {
 while (inSize-- > 0) {
 int32_t x = *in++;

 int32_t mantissa = ~x;
 int32_t exponent = (mantissa >> 4) & 7;
 int32_t segment = exponent + 1;
        mantissa &= 0x0f;

 int32_t step = 4 << segment;

 int32_t abs = (0x80l << exponent) + step * mantissa + step / 2 - 4 * 33;

 *out++ = (x < 0x80) ? -abs : abs;
 }
}
