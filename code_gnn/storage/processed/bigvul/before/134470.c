bool HomeButton::GetDropFormats(
    int* formats,
    std::set<OSExchangeData::CustomFormat>* custom_formats) {
  *formats = ui::OSExchangeData::URL;
  return true;
}
