void BrowserTestClipboardScope::GetText(std::string* result) {
  ui::Clipboard::GetForCurrentThread()->ReadAsciiText(
      ui::CLIPBOARD_TYPE_COPY_PASTE, result);
}
