void AutomationProvider::SetWindowVisible(int handle, bool visible,
                                          bool* result) {
  *result = false;
  GtkWindow* window = window_tracker_->GetResource(handle);
  if (window) {
    if (visible) {
      gtk_window_present(window);
    } else {
      gtk_widget_hide(GTK_WIDGET(window));
    }
    *result = true;
  }
}
