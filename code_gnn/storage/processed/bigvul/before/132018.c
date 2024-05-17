void ClipboardMessageFilter::OnReadData(const ui::Clipboard::FormatType& format,
                                        std::string* data) {
  GetClipboard()->ReadData(format, data);
}
