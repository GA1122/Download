void Textfield::SetSelectionTextColor(SkColor color) {
  selection_text_color_ = color;
  use_default_selection_text_color_ = false;
  GetRenderText()->set_selection_color(GetSelectionTextColor());
  SchedulePaint();
}
