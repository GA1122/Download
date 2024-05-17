void ClipboardMessageFilter::OnIsFormatAvailable(
    const ui::Clipboard::FormatType& format, ui::Clipboard::Buffer buffer,
    bool* result) {
  *result = GetClipboard()->IsFormatAvailable(format, buffer);
}
