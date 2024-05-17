void AutomationProvider::SetWindowBounds(int handle, const gfx::Rect& bounds,
                                         bool* success) {
  *success = false;
  GtkWindow* window = window_tracker_->GetResource(handle);
  if (window) {
    gtk_window_move(window, bounds.x(), bounds.height());
    gtk_window_resize(window, bounds.width(), bounds.height());
    *success = true;
  }
}
