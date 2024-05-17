void BaseRenderingContext2D::DrawPathInternal(
    const Path& path,
    CanvasRenderingContext2DState::PaintType paint_type,
    SkPath::FillType fill_type) {
  if (path.IsEmpty())
    return;

  SkPath sk_path = path.GetSkPath();
  FloatRect bounds = path.BoundingRect();
  sk_path.setFillType(fill_type);

  if (paint_type == CanvasRenderingContext2DState::kStrokePaintType)
    InflateStrokeRect(bounds);

  if (!DrawingCanvas())
    return;

  Draw([&sk_path](PaintCanvas* c, const PaintFlags* flags)   
       { c->drawPath(sk_path, *flags); },
       [](const SkIRect& rect)   
       { return false; },
       bounds, paint_type);
}
