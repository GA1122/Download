void ClipboardMessageFilter::OnReadAsciiText(ui::ClipboardType type,
                                             std::string* result) {
  GetClipboard()->ReadAsciiText(type, result);
}
