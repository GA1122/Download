SkColor TabStrip::GetTabBackgroundColor(
    TabState tab_state,
    BrowserNonClientFrameView::ActiveState active_state) const {
  const ui::ThemeProvider* tp = GetThemeProvider();
  if (!tp)
    return SK_ColorBLACK;

  if (tab_state == TAB_ACTIVE)
    return tp->GetColor(ThemeProperties::COLOR_TOOLBAR);

  bool is_active_frame;
  if (active_state == BrowserNonClientFrameView::kUseCurrent)
    is_active_frame = ShouldPaintAsActiveFrame();
  else
    is_active_frame = active_state == BrowserNonClientFrameView::kActive;

  const int color_id = is_active_frame
                           ? ThemeProperties::COLOR_BACKGROUND_TAB
                           : ThemeProperties::COLOR_BACKGROUND_TAB_INACTIVE;
  const SkColor frame = controller_->GetFrameColor(active_state);
  const SkColor background =
      tp->HasCustomColor(color_id)
          ? tp->GetColor(color_id)
          : color_utils::HSLShift(
                frame, tp->GetTint(ThemeProperties::TINT_BACKGROUND_TAB));

  return color_utils::GetResultingPaintColor(background, frame);
}
