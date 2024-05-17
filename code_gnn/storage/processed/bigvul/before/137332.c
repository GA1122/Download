base::string16 GetClipboardText(ui::ClipboardType type) {
  base::string16 text;
  ui::Clipboard::GetForCurrentThread()->ReadText(type, &text);
  return text;
}
