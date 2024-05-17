unsigned short ConvertFloatToHalfFloat(float f) {
  unsigned temp = *(reinterpret_cast<unsigned*>(&f));
  unsigned signexp = (temp >> 23) & 0x1ff;
  return g_base_table[signexp] +
         ((temp & 0x007fffff) >> g_shift_table[signexp]);
}
