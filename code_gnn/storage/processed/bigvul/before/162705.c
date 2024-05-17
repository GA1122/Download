bool BaseRenderingContext2D::ShouldDrawImageAntialiased(
    const FloatRect& dest_rect) const {
  if (!GetState().ShouldAntialias())
    return false;
  PaintCanvas* c = DrawingCanvas();
  DCHECK(c);

  const SkMatrix& ctm = c->getTotalMatrix();
  if (!ctm.rectStaysRect())
    return true;
  SkScalar width_expansion, height_expansion;
  if (ctm.getType() & SkMatrix::kAffine_Mask) {
    width_expansion = ctm[SkMatrix::kMSkewY];
    height_expansion = ctm[SkMatrix::kMSkewX];
  } else {
    width_expansion = ctm[SkMatrix::kMScaleX];
    height_expansion = ctm[SkMatrix::kMScaleY];
  }
  return dest_rect.Width() * fabs(width_expansion) < 1 ||
         dest_rect.Height() * fabs(height_expansion) < 1;
}
