void ClipboardMessageFilter::OnReadRTF(ui::ClipboardType type,
                                       std::string* result) {
  GetClipboard()->ReadRTF(type, result);
}
