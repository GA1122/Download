void BrowserWindowGtk::OnDebouncedBoundsChanged() {
  gtk_window_util::UpdateWindowPosition(this, &bounds_, &restored_bounds_);
  SaveWindowPosition();
}
