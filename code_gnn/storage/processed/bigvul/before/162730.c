void BaseRenderingContext2D::fill(Path2D* dom_path,
                                  const String& winding_rule_string) {
  DrawPathInternal(dom_path->GetPath(),
                   CanvasRenderingContext2DState::kFillPaintType,
                   ParseWinding(winding_rule_string));
}
