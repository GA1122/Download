void BaseRenderingContext2D::clip(Path2D* dom_path,
                                  const String& winding_rule_string) {
  ClipInternal(dom_path->GetPath(), winding_rule_string);
}
