void PlatformFontSkia::SetDefaultFontDescription(
    const std::string& font_description) {
  delete default_font_description_;
  default_font_description_ = new std::string(font_description);
}
