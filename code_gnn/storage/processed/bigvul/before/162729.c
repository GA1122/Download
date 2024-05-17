void BaseRenderingContext2D::fill(const String& winding_rule_string) {
  DrawPathInternal(path_, CanvasRenderingContext2DState::kFillPaintType,
                   ParseWinding(winding_rule_string));
}
