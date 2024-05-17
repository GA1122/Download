void Textfield::UseDefaultSelectionBackgroundColor() {
  use_default_selection_background_color_ = true;
  GetRenderText()->set_selection_background_focused_color(
      GetSelectionBackgroundColor());
  SchedulePaint();
}
