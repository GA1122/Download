void BaseRenderingContext2D::scale(double sx, double sy) {
  PaintCanvas* c = DrawingCanvas();
  if (!c)
    return;

  if (!std::isfinite(sx) || !std::isfinite(sy))
    return;

  AffineTransform new_transform = GetState().Transform();
  new_transform.ScaleNonUniform(sx, sy);
  if (GetState().Transform() == new_transform)
    return;

  ModifiableState().SetTransform(new_transform);
  if (!GetState().IsTransformInvertible())
    return;

  c->scale(sx, sy);
  path_.Transform(AffineTransform().ScaleNonUniform(1.0 / sx, 1.0 / sy));
}
