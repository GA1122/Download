String BaseRenderingContext2D::lineCap() const {
  return LineCapName(GetState().GetLineCap());
}
