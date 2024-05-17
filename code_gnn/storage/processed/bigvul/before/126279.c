bool BrowserCommandController::IsShowingMainUI(bool is_fullscreen) {
#if !defined(OS_MACOSX)
  return browser_->is_type_tabbed() && !is_fullscreen;
#else
  return browser_->is_type_tabbed();
#endif
}
