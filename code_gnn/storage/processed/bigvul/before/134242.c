void OmniboxViewViews::AppendDropFormats(
    int* formats,
    std::set<ui::OSExchangeData::CustomFormat>* custom_formats) {
  *formats = *formats | ui::OSExchangeData::URL;
}
