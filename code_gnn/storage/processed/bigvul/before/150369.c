void ClientControlledShellSurface::InitializeWindowState(
    ash::wm::WindowState* window_state) {
  window_state->set_allow_set_bounds_direct(true);
  window_state->set_ignore_keyboard_bounds_change(true);
  if (container_ == ash::kShellWindowId_SystemModalContainer)
    DisableMovement();
  ash::NonClientFrameViewAsh* frame_view = GetFrameView();
  frame_view->SetCaptionButtonModel(std::make_unique<CaptionButtonModel>(
      frame_visible_button_mask_, frame_enabled_button_mask_));
  UpdateAutoHideFrame();
  UpdateFrameWidth();
  if (initial_orientation_lock_ != ash::OrientationLockType::kAny)
    SetOrientationLock(initial_orientation_lock_);
}
