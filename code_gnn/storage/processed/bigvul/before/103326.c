void ClipboardMessageFilter::OnReadData(
    ui::Clipboard::Buffer buffer, const string16& type, bool* succeeded,
    string16* data, string16* metadata) {
  *succeeded = ClipboardDispatcher::ReadData(buffer, type, data, metadata);
}
