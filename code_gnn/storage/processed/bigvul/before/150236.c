gfx::Rect GetBoundsInTabletMode(wm::WindowState* state_object) {
  if (state_object->IsFullscreen() || state_object->IsPinned()) {
    return screen_util::GetFullscreenWindowBoundsInParent(
        state_object->window());
  }

  if (state_object->GetStateType() == WindowStateType::kLeftSnapped) {
    return Shell::Get()
        ->split_view_controller()
        ->GetSnappedWindowBoundsInParent(state_object->window(),
                                         SplitViewController::LEFT);
  }

  if (state_object->GetStateType() == WindowStateType::kRightSnapped) {
    return Shell::Get()
        ->split_view_controller()
        ->GetSnappedWindowBoundsInParent(state_object->window(),
                                         SplitViewController::RIGHT);
  }

  gfx::Rect bounds_in_parent;

  if ((state_object->CanMaximize() || state_object->CanResize()) &&
      ::wm::GetTransientParent(state_object->window()) == nullptr) {
    bounds_in_parent.set_size(GetMaximumSizeOfWindow(state_object));
  } else {
    if (state_object->HasRestoreBounds())
      bounds_in_parent = state_object->GetRestoreBoundsInParent();
    else
      bounds_in_parent = state_object->window()->bounds();
  }
  return GetCenteredBounds(bounds_in_parent, state_object);
}
