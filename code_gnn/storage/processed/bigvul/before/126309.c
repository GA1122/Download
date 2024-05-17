void BrowserWindowGtk::ActiveWindowChanged(GdkWindow* active_window) {
  if (!window_)
    return;

  bool is_active = gtk_widget_get_window(GTK_WIDGET(window_)) == active_window;
  bool changed = (is_active != is_active_);

  if (is_active && changed) {
    if (AppModalDialogQueue::GetInstance()->HasActiveDialog()) {
      AppModalDialogQueue::GetInstance()->ActivateModalDialog();
      return;
    }
  }

  is_active_ = is_active;
  if (changed) {
    SetBackgroundColor();
    InvalidateWindow();
    UpdateWindowShape(bounds_.width(), bounds_.height());
  }
}
