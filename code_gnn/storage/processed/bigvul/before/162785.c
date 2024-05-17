void BaseRenderingContext2D::transform(double m11,
                                       double m12,
                                       double m21,
                                       double m22,
                                       double dx,
                                       double dy) {
  PaintCanvas* c = DrawingCanvas();
  if (!c)
    return;

  if (!std::isfinite(m11) || !std::isfinite(m21) || !std::isfinite(dx) ||
      !std::isfinite(m12) || !std::isfinite(m22) || !std::isfinite(dy))
    return;

  AffineTransform transform(m11, m12, m21, m22, dx, dy);
  AffineTransform new_transform = GetState().Transform() * transform;
  if (GetState().Transform() == new_transform)
    return;

  ModifiableState().SetTransform(new_transform);
  if (!GetState().IsTransformInvertible())
    return;

  c->concat(AffineTransformToSkMatrix(transform));
  path_.Transform(transform.Inverse());
}
