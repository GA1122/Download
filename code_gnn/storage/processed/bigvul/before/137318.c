void Textfield::UseDefaultSelectionTextColor() {
  use_default_selection_text_color_ = true;
  GetRenderText()->set_selection_color(GetSelectionTextColor());
  SchedulePaint();
}
