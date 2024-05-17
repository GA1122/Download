bool Textfield::CanDrop(const OSExchangeData& data) {
  int formats;
  std::set<ui::Clipboard::FormatType> format_types;
  GetDropFormats(&formats, &format_types);
  return enabled() && !read_only() && data.HasAnyFormat(formats, format_types);
}
