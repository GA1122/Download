void Textfield::SetSelectionBackgroundColor(SkColor color) {
  selection_background_color_ = color;
  use_default_selection_background_color_ = false;
  GetRenderText()->set_selection_background_focused_color(
      GetSelectionBackgroundColor());
  SchedulePaint();
}
