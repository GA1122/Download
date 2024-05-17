void Label::SetHasFocusBorder(bool has_focus_border) {
  has_focus_border_ = has_focus_border;
  if (is_multi_line_) {
    text_size_valid_ = false;
    PreferredSizeChanged();
  }
}
