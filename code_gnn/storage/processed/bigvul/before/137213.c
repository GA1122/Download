SkColor Textfield::GetTextColor() const {
  if (!use_default_text_color_)
    return text_color_;

  return style::GetColor(*this, style::CONTEXT_TEXTFIELD, GetTextStyle());
}
