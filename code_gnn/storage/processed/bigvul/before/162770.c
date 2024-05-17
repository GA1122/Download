void BaseRenderingContext2D::setShadowOffsetX(double x) {
  if (!std::isfinite(x))
    return;
  if (GetState().ShadowOffset().Width() == x)
    return;
  ModifiableState().SetShadowOffsetX(x);
}
