void Label::Init(const std::wstring& text, const gfx::Font& font) {
  static bool initialized = false;
  if (!initialized) {
#if defined(OS_WIN)
    kEnabledColor = color_utils::GetSysSkColor(COLOR_WINDOWTEXT);
    kDisabledColor = color_utils::GetSysSkColor(COLOR_GRAYTEXT);
#else
    kEnabledColor = SK_ColorBLACK;
    kDisabledColor = SK_ColorGRAY;
#endif

    initialized = true;
  }

  contains_mouse_ = false;
  font_ = font;
  text_size_valid_ = false;
  SetText(text);
  url_set_ = false;
  color_ = kEnabledColor;
  horiz_alignment_ = ALIGN_CENTER;
  is_multi_line_ = false;
  allow_character_break_ = false;
  elide_in_middle_ = false;
  collapse_when_hidden_ = false;
  rtl_alignment_mode_ = USE_UI_ALIGNMENT;
  paint_as_focused_ = false;
  has_focus_border_ = false;
}
