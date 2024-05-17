SkColor GetThemeColor(ui::ThemeProvider* tp, int id) {
  SkColor color = tp->GetColor(id);
  return gfx::IsInvertedColorScheme() ?
      color_utils::InvertColor(color) : color;
}
