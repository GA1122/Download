void BrowserTestClipboardScope::SetRtf(const std::string& rtf) {
  ui::ScopedClipboardWriter clipboard_writer(ui::CLIPBOARD_TYPE_COPY_PASTE);
  clipboard_writer.WriteRTF(rtf);
}
