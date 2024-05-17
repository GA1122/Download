SkColor Textfield::GetBackgroundColor() const {
  if (!use_default_background_color_)
    return background_color_;

  return GetNativeTheme()->GetSystemColor(
      read_only() || !enabled()
          ? ui::NativeTheme::kColorId_TextfieldReadOnlyBackground
          : ui::NativeTheme::kColorId_TextfieldDefaultBackground);
}
