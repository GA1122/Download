void BaseRenderingContext2D::stroke(Path2D* dom_path) {
  DrawPathInternal(dom_path->GetPath(),
                   CanvasRenderingContext2DState::kStrokePaintType);
}
