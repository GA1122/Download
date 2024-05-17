void BrowserTestClipboardScope::SetText(const std::string& text) {
  ui::ScopedClipboardWriter clipboard_writer(ui::CLIPBOARD_TYPE_COPY_PASTE);
  clipboard_writer.WriteText(base::ASCIIToUTF16(text));
}
