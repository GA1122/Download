bool BaseRenderingContext2D::IsPointInStrokeInternal(const Path& path,
                                                     const double x,
                                                     const double y) {
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

  StrokeData stroke_data;
  stroke_data.SetThickness(GetState().LineWidth());
  stroke_data.SetLineCap(GetState().GetLineCap());
  stroke_data.SetLineJoin(GetState().GetLineJoin());
  stroke_data.SetMiterLimit(GetState().MiterLimit());
  Vector<float> line_dash(GetState().LineDash().size());
  std::copy(GetState().LineDash().begin(), GetState().LineDash().end(),
            line_dash.begin());
  stroke_data.SetLineDash(line_dash, GetState().LineDashOffset());
  return path.StrokeContains(transformed_point, stroke_data);
}
