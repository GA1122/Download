void ShelfLayoutManager::OnKeyboardVisibilityStateChanged(
    const bool is_visible) {
  if (Shell::Get()->session_controller()->IsUserSessionBlocked() &&
      !is_visible) {
    Shell::Get()->SetDisplayWorkAreaInsets(shelf_widget_->GetNativeWindow(),
                                           gfx::Insets());
  }
}
