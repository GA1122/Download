void Tab::UpdateForegroundColors() {
  TabStyle::TabColors colors = tab_style_->CalculateColors();

  icon_->SetBackgroundColor(colors.background_color);
  title_->SetEnabledColor(colors.title_color);

  close_button_->SetIconColors(
      colors.button_icon_idle_color, colors.button_icon_hovered_color,
      colors.button_icon_hovered_color, colors.button_background_hovered_color,
      colors.button_background_pressed_color);

  if (button_color_ != colors.button_icon_idle_color) {
    button_color_ = colors.button_icon_idle_color;
    alert_indicator_->OnParentTabButtonColorChanged();
  }

  SchedulePaint();
}
