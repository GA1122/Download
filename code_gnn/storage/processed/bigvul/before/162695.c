bool BaseRenderingContext2D::IsPointInPathInternal(
    const Path& path,
    const double x,
    const double y,
    const String& winding_rule_string) {
  PaintCanvas* c = DrawingCanvas();
  if (!c)
    return false;
  if (!GetState().IsTransformInvertible())
    return false;

  FloatPoint point(x, y);
  if (!std::isfinite(point.X()) || !std::isfinite(point.Y()))
    return false;
  AffineTransform ctm = GetState().Transform();
  FloatPoint transformed_point = ctm.Inverse().MapPoint(point);

  return path.Contains(transformed_point,
                       SkFillTypeToWindRule(ParseWinding(winding_rule_string)));
}
