CanvasRenderingContext2DState& BaseRenderingContext2D::ModifiableState() {
  RealizeSaves();
  return *state_stack_.back();
}
