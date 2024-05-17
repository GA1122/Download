void ClipboardMessageFilter::OnReadText(ui::ClipboardType type,
                                        base::string16* result) {
  GetClipboard()->ReadText(type, result);
}
