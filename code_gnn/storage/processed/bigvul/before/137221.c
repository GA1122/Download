int Textfield::GetTextStyle() const {
  return (read_only() || !enabled()) ? style::STYLE_DISABLED
                                     : style::STYLE_PRIMARY;
}
