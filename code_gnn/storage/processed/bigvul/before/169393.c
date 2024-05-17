BrowserTestClipboardScope::~BrowserTestClipboardScope() {
  ui::Clipboard::DestroyClipboardForCurrentThread();
}
