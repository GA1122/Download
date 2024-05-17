void Label::OnBoundsChanged(const gfx::Rect& previous_bounds) {
  text_size_valid_ &= !is_multi_line_;
}
