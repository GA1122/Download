void WindowTreeHostManager::CloseMirroringDisplayIfNotNecessary() {
  mirror_window_controller_->CloseIfNotNecessary();
  if (!cursor_window_controller_->is_cursor_compositing_enabled())
    cursor_window_controller_->UpdateContainer();
}
