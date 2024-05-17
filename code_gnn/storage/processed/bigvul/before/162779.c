void BaseRenderingContext2D::stroke() {
  DrawPathInternal(path_, CanvasRenderingContext2DState::kStrokePaintType);
}
