float BaseRenderingContext2D::GetFontBaseline(
    const FontMetrics& font_metrics) const {
  return TextMetrics::GetFontBaseline(GetState().GetTextBaseline(),
                                      font_metrics);
}
