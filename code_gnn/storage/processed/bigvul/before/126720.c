bool BrowserView::CanClose() {
  if (tabstrip_ && !tabstrip_->IsTabStripCloseable())
    return false;

  if (!browser_->ShouldCloseWindow())
    return false;

  if (!browser_->tab_strip_model()->empty()) {
    frame_->Hide();
    browser_->OnWindowClosing();
    return false;
  }

  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_WINDOW_CLOSED,
      content::Source<gfx::NativeWindow>(frame_->GetNativeWindow()),
      content::NotificationService::NoDetails());
  return true;
}
