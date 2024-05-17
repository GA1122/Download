gfx::PointF CssPixelsToPointF(double x, double y, float page_scale_factor) {
  return gfx::PointF(x * page_scale_factor, y * page_scale_factor);
}
