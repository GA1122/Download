void TabletModeWindowState::UpdateWindow(wm::WindowState* window_state,
                                         WindowStateType target_state,
                                         bool animated) {
  DCHECK(target_state == WindowStateType::kMinimized ||
         target_state == WindowStateType::kMaximized ||
         target_state == WindowStateType::kPinned ||
         target_state == WindowStateType::kTrustedPinned ||
         (target_state == WindowStateType::kNormal &&
          (!window_state->CanMaximize() ||
           !!::wm::GetTransientParent(window_state->window()))) ||
         target_state == WindowStateType::kFullscreen ||
         target_state == WindowStateType::kLeftSnapped ||
         target_state == WindowStateType::kRightSnapped);

  if (current_state_type_ == target_state) {
    if (target_state == WindowStateType::kMinimized)
      return;
    UpdateBounds(window_state, animated);
    return;
  }

  const WindowStateType old_state_type = current_state_type_;
  current_state_type_ = target_state;
  window_state->UpdateWindowPropertiesFromStateType();
  window_state->NotifyPreStateTypeChange(old_state_type);

  if (target_state == WindowStateType::kMinimized) {
    ::wm::SetWindowVisibilityAnimationType(
        window_state->window(), wm::WINDOW_VISIBILITY_ANIMATION_TYPE_MINIMIZE);
    window_state->window()->Hide();
    if (window_state->IsActive())
      window_state->Deactivate();
  } else {
    UpdateBounds(window_state, animated);
  }

  if ((window_state->window()->layer()->GetTargetVisibility() ||
       old_state_type == WindowStateType::kMinimized) &&
      !window_state->window()->layer()->visible()) {
    window_state->window()->Show();
  }

  window_state->NotifyPostStateTypeChange(old_state_type);

  if (old_state_type == WindowStateType::kPinned ||
      target_state == WindowStateType::kPinned ||
      old_state_type == WindowStateType::kTrustedPinned ||
      target_state == WindowStateType::kTrustedPinned) {
    Shell::Get()->screen_pinning_controller()->SetPinnedWindow(
        window_state->window());
  }
}
