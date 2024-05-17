bool BaseRenderingContext2D::IsFullCanvasCompositeMode(SkBlendMode op) {
  return op == SkBlendMode::kSrcIn || op == SkBlendMode::kSrcOut ||
         op == SkBlendMode::kDstIn || op == SkBlendMode::kDstATop;
}
