void LockContentsView::OnLockScreenNoteStateChanged(
    mojom::TrayActionState state) {
  if (disable_lock_screen_note_)
    state = mojom::TrayActionState::kNotAvailable;

  bool old_lock_screen_apps_active = lock_screen_apps_active_;
  lock_screen_apps_active_ = state == mojom::TrayActionState::kActive;
  note_action_->UpdateVisibility(state);
  LayoutTopHeader();

  if (old_lock_screen_apps_active && !lock_screen_apps_active_ &&
      primary_big_view_) {
    primary_big_view_->RequestFocus();
  }
}
