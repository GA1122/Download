bool BrowserView::CanClose() {
  if (tabstrip_ && !tabstrip_->IsTabStripCloseable())
    return false;

  if (!browser_->ShouldCloseWindow())
    return false;

  bool fast_tab_closing_enabled =
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableFastUnload);

  if (!browser_->tab_strip_model()->empty()) {
    frame_->Hide();
    browser_->OnWindowClosing();
    if (fast_tab_closing_enabled)
      browser_->tab_strip_model()->CloseAllTabs();
    return false;
  } else if (fast_tab_closing_enabled &&
        !browser_->HasCompletedUnloadProcessing()) {
    frame_->Hide();
    return false;
  }

  return true;
}
