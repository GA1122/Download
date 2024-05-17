void BaseRenderingContext2D::resetTransform() {
  PaintCanvas* c = DrawingCanvas();
  if (!c)
    return;

  AffineTransform ctm = GetState().Transform();
  bool invertible_ctm = GetState().IsTransformInvertible();
  if (ctm.IsIdentity() && invertible_ctm)
    return;

  ModifiableState().ResetTransform();
  c->setMatrix(AffineTransformToSkMatrix(AffineTransform()));

  if (invertible_ctm)
    path_.Transform(ctm);
}
