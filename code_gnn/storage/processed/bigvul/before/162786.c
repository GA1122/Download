void BaseRenderingContext2D::translate(double tx, double ty) {
  PaintCanvas* c = DrawingCanvas();
  if (!c)
    return;
  if (!GetState().IsTransformInvertible())
    return;

  if (!std::isfinite(tx) || !std::isfinite(ty))
    return;

  AffineTransform new_transform = GetState().Transform();
  new_transform.Translate(tx, ty);
  if (GetState().Transform() == new_transform)
    return;

  ModifiableState().SetTransform(new_transform);
  if (!GetState().IsTransformInvertible())
    return;
  c->translate(tx, ty);
  path_.Transform(AffineTransform().Translate(-tx, -ty));
}
