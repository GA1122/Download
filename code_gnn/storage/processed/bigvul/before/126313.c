bool BrowserWindowGtk::CanClose() const {
  if (tabstrip_->IsDragSessionActive())
    return false;

  if (!browser_->ShouldCloseWindow())
    return false;

  if (!browser_->tab_strip_model()->empty()) {
    gtk_widget_hide(GTK_WIDGET(window_));
    browser_->OnWindowClosing();
    return false;
  }

  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_WINDOW_CLOSED,
      content::Source<GtkWindow>(window_),
      content::NotificationService::NoDetails());
  return true;
}
