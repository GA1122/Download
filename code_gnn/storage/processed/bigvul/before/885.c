void CairoOutputDev::clearSoftMask(GfxState *  ) {
  if (mask)
    cairo_pattern_destroy(mask);
  mask = NULL;
}
