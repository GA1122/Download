String BaseRenderingContext2D::shadowColor() const {
  return Color(GetState().ShadowColor()).Serialized();
}
