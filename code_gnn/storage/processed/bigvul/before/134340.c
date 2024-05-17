base::string16 GetClipboardText() {
  if (!ui::Clipboard::IsSupportedClipboardType(ui::CLIPBOARD_TYPE_SELECTION))
    return base::string16();
  ui::Clipboard* clipboard = ui::Clipboard::GetForCurrentThread();
  CHECK(clipboard);
  base::string16 clipboard_text;
  clipboard->ReadText(ui::CLIPBOARD_TYPE_SELECTION, &clipboard_text);
  return clipboard_text;
}
