void RenderWidgetHostViewAura::UpdateBackgroundColorFromRenderer(
    SkColor color) {
  if (color == background_color())
    return;
  background_color_ = color;

  bool opaque = SkColorGetA(color) == SK_AlphaOPAQUE;
  window_->layer()->SetFillsBoundsOpaquely(opaque);
  window_->layer()->SetColor(color);
}
