SkColor TabStrip::GetTabForegroundColor(TabState tab_state,
                                        SkColor background_color) const {
  const ui::ThemeProvider* tp = GetThemeProvider();
  if (!tp)
    return SK_ColorBLACK;

  const bool is_active_frame = ShouldPaintAsActiveFrame();

  SkColor default_color;
  if (tab_state == TAB_ACTIVE) {
    default_color = tp->GetColor(ThemeProperties::COLOR_TAB_TEXT);
  } else {
    if (!is_active_frame &&
        tp->HasCustomColor(
            ThemeProperties::COLOR_BACKGROUND_TAB_TEXT_INACTIVE)) {
      return tp->GetColor(ThemeProperties::COLOR_BACKGROUND_TAB_TEXT_INACTIVE);
    }

    const int color_id = ThemeProperties::COLOR_BACKGROUND_TAB_TEXT;
    default_color =
        tp->HasCustomColor(color_id)
            ? tp->GetColor(color_id)
            : color_utils::PickContrastingColor(
                  gfx::kGoogleGrey400, gfx::kGoogleGrey800, background_color);
  }

  if (!is_active_frame) {
    default_color =
        color_utils::AlphaBlend(default_color, background_color, 0.75f);
  }

  return color_utils::GetColorWithMinimumContrast(default_color,
                                                  background_color);
}
