void AutocompletePopupViewGtk::PaintUpdatesNow() {
  gdk_window_process_updates(window_->window, FALSE);
}
