void Shell::SetCursorCompositingEnabled(bool enabled) {
  window_tree_host_manager_->cursor_window_controller()
      ->SetCursorCompositingEnabled(enabled);
  native_cursor_manager_->SetNativeCursorEnabled(!enabled);
}
