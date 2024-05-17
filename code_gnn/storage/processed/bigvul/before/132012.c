void ClipboardMessageFilter::OnClear(ui::ClipboardType type) {
  GetClipboard()->Clear(type);
}
