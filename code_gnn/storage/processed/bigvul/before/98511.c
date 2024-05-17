gfx::Rect GetRectForLine(size_t line, int width) {
  return gfx::Rect(kBorderThickness,
                   (line * kHeightPerResult) + kBorderThickness,
                   width - (kBorderThickness * 2),
                   kHeightPerResult);
}
