void BaseRenderingContext2D::rotate(double angle_in_radians) {
  PaintCanvas* c = DrawingCanvas();
  if (!c)
    return;

  if (!std::isfinite(angle_in_radians))
    return;

  AffineTransform new_transform = GetState().Transform();
  new_transform.RotateRadians(angle_in_radians);
  if (GetState().Transform() == new_transform)
    return;

  ModifiableState().SetTransform(new_transform);
  if (!GetState().IsTransformInvertible())
    return;
  c->rotate(angle_in_radians * (180.0 / piFloat));
  path_.Transform(AffineTransform().RotateRadians(-angle_in_radians));
}
