GdkColor SelectedURLColor(GdkColor foreground, GdkColor background) {
  color_utils::HSL fg_hsl;
  color_utils::SkColorToHSL(gfx::GdkColorToSkColor(foreground), &fg_hsl);

  color_utils::HSL bg_hsl;
  color_utils::SkColorToHSL(gfx::GdkColorToSkColor(background), &bg_hsl);

  color_utils::HSL hue_hsl;
  color_utils::SkColorToHSL(gfx::GdkColorToSkColor(kURLTextColor), &hue_hsl);

  double opposite_s = 1 - bg_hsl.s;
  double s = std::max(0.2, std::min(0.8, opposite_s));

  double opposite_l = fg_hsl.l;
  double l = std::max(0.1, std::min(0.9, opposite_l));

  color_utils::HSL output = { hue_hsl.h, s, l };
  return gfx::SkColorToGdkColor(color_utils::HSLToSkColor(output, 255));
}
