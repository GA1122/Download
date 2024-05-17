CanvasGradient* BaseRenderingContext2D::createRadialGradient(
    double x0,
    double y0,
    double r0,
    double x1,
    double y1,
    double r1,
    ExceptionState& exception_state) {
  if (r0 < 0 || r1 < 0) {
    exception_state.ThrowDOMException(
        kIndexSizeError, String::Format("The %s provided is less than 0.",
                                        r0 < 0 ? "r0" : "r1"));
    return nullptr;
  }

  CanvasGradient* gradient =
      CanvasGradient::Create(FloatPoint(x0, y0), r0, FloatPoint(x1, y1), r1);
  return gradient;
}
