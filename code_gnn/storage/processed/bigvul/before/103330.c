void ClipboardMessageFilter::OnReadText(
    ui::Clipboard::Buffer buffer, string16* result) {
  GetClipboard()->ReadText(buffer, result);
}
