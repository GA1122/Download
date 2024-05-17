void BaseRenderingContext2D::setShadowOffsetY(double y) {
  if (!std::isfinite(y))
    return;
  if (GetState().ShadowOffset().Height() == y)
    return;
  ModifiableState().SetShadowOffsetY(y);
}
