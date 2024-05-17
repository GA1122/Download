void ClipboardMessageFilter::OnReadAsciiText(
    ui::Clipboard::Buffer buffer, std::string* result) {
  GetClipboard()->ReadAsciiText(buffer, result);
}
