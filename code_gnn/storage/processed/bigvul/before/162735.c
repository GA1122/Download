const Vector<double>& BaseRenderingContext2D::getLineDash() const {
  return GetState().LineDash();
}
