CanvasGradient* BaseRenderingContext2D::createLinearGradient(double x0,
                                                             double y0,
                                                             double x1,
                                                             double y1) {
  CanvasGradient* gradient =
      CanvasGradient::Create(FloatPoint(x0, y0), FloatPoint(x1, y1));
  return gradient;
}
