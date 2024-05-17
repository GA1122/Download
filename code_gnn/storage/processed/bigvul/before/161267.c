gfx::Vector2dF CssPixelsToVector2dF(double x,
                                    double y,
                                    float page_scale_factor) {
  return gfx::Vector2dF(x * page_scale_factor, y * page_scale_factor);
}
