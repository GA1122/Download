void BrowserMainLoop::PreShutdown() {
  parts_->PreShutdown();

  ui::Clipboard::OnPreShutdownForCurrentThread();
}
