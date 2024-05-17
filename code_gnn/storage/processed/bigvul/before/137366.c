void SetClipboardText(ui::ClipboardType type, const std::string& text) {
  ui::ScopedClipboardWriter(type).WriteText(ASCIIToUTF16(text));
}
