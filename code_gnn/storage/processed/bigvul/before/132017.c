void ClipboardMessageFilter::OnReadCustomData(ui::ClipboardType clipboard_type,
                                              const base::string16& type,
                                              base::string16* result) {
  GetClipboard()->ReadCustomData(clipboard_type, type, result);
}
