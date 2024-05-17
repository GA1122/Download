 static inline void ConvertCanvasStyleToUnionType(
    CanvasStyle* style,
    StringOrCanvasGradientOrCanvasPattern& return_value) {
  if (CanvasGradient* gradient = style->GetCanvasGradient()) {
    return_value.SetCanvasGradient(gradient);
    return;
  }
  if (CanvasPattern* pattern = style->GetCanvasPattern()) {
    return_value.SetCanvasPattern(pattern);
    return;
  }
  return_value.SetString(style->GetColor());
}
