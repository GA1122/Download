void OmniboxViewViews::AppendDropFormats(
    int* formats,
    std::set<ui::Clipboard::FormatType>* format_types) {
  *formats = *formats | ui::OSExchangeData::URL;
}
