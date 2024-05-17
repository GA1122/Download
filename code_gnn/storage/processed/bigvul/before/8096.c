static inline GBool isSameGfxColor(const GfxColor &colorA, const GfxColor &colorB, Guint nComps, double delta) {
  for (Guint k = 0; k < nComps; ++k) {
    if (abs(colorA.c[k] - colorB.c[k]) > delta) {
      return false;
    }
  }
  return true;
}
