  void CalculateCircleTransforms(const gfx::Size& size,
                                 PaintedShapeTransforms* transforms_out) const {
    CalculateRectTransforms(size, std::min(size.width(), size.height()) / 2.0f,
                            transforms_out);
  }
