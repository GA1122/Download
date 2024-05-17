static FloatRoundedRect ToClipRect(const LayoutRect& rect) {
  if (RuntimeEnabledFeatures::SlimmingPaintV175Enabled())
    return FloatRoundedRect(FloatRect(PixelSnappedIntRect(rect)));
  return FloatRoundedRect(FloatRect(rect));
}
