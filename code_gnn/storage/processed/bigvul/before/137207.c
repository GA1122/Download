SkColor Textfield::GetSelectionBackgroundColor() const {
  return use_default_selection_background_color_
             ? GetNativeTheme()->GetSystemColor(
                   ui::NativeTheme::
                       kColorId_TextfieldSelectionBackgroundFocused)
             : selection_background_color_;
}
