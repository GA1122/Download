void ClipboardMessageFilter::OnIsFormatAvailable(
    const ui::Clipboard::FormatType& format,
    ui::ClipboardType type,
    bool* result) {
  *result = GetClipboard()->IsFormatAvailable(format, type);
}
