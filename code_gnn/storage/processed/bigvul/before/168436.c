PlatformFont* PlatformFont::CreateFromNameAndSize(const std::string& font_name,
                                                  int font_size) {
  TRACE_EVENT0("fonts", "PlatformFont::CreateFromNameAndSize");
  return new PlatformFontSkia(font_name, font_size);
}
