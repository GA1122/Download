void BaseRenderingContext2D::strokeRect(double x,
                                        double y,
                                        double width,
                                        double height) {
  if (!ValidateRectForCanvas(x, y, width, height))
    return;

  if (!DrawingCanvas())
    return;

  SkRect rect = SkRect::MakeXYWH(x, y, width, height);
  FloatRect bounds = rect;
  InflateStrokeRect(bounds);
  Draw([&rect](PaintCanvas* c, const PaintFlags* flags)   
       { StrokeRectOnCanvas(rect, c, flags); },
       [](const SkIRect& clip_bounds)   
       { return false; },
       bounds, CanvasRenderingContext2DState::kStrokePaintType);
}
