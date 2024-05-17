void BrowserMainLoop::PreMainMessageLoopStart() {
  if (parts_) {
    TRACE_EVENT0("startup",
        "BrowserMainLoop::MainMessageLoopStart:PreMainMessageLoopStart");
    parts_->PreMainMessageLoopStart();
  }

#if defined(OS_WIN)
  if (!parameters_.ui_task) {
    l10n_util::OverrideLocaleWithUILanguageList();
  }
#endif
}
