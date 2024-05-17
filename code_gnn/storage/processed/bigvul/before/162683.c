void BaseRenderingContext2D::ClipInternal(const Path& path,
                                          const String& winding_rule_string) {
  PaintCanvas* c = DrawingCanvas();
  if (!c) {
    return;
  }
  if (!GetState().IsTransformInvertible()) {
    return;
  }

  SkPath sk_path = path.GetSkPath();
  sk_path.setFillType(ParseWinding(winding_rule_string));
  ModifiableState().ClipPath(sk_path, clip_antialiasing_);
  c->clipPath(sk_path, SkClipOp::kIntersect,
              clip_antialiasing_ == kAntiAliased);
}
