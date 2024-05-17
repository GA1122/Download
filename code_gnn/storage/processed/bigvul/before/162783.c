String BaseRenderingContext2D::textAlign() const {
  return TextAlignName(GetState().GetTextAlign());
}
