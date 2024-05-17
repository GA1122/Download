SVGMatrixTearOff* BaseRenderingContext2D::currentTransform() const {
  return SVGMatrixTearOff::Create(GetState().Transform());
}
