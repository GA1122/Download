void BaseRenderingContext2D::clip(const String& winding_rule_string) {
  ClipInternal(path_, winding_rule_string);
}
