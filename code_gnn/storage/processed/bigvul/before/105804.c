void Label::SetFont(const gfx::Font& font) {
  font_ = font;
  text_size_valid_ = false;
  PreferredSizeChanged();
  SchedulePaint();
}
