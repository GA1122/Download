void TabletModeWindowState::UpdateBounds(wm::WindowState* window_state,
                                         bool animated) {
  if (wm::IsDraggingTabs(window_state->window()))
    return;

  if (current_state_type_ == WindowStateType::kMinimized)
    return;

  gfx::Rect bounds_in_parent = GetBoundsInTabletMode(window_state);
  if (!bounds_in_parent.IsEmpty() &&
      bounds_in_parent != window_state->window()->bounds()) {
    if (!window_state->window()->IsVisible() || !animated) {
      window_state->SetBoundsDirect(bounds_in_parent);
    } else {
      if (enter_animation_type() == STEP_END) {
        window_state->SetBoundsDirectAnimated(bounds_in_parent,
                                              base::TimeDelta::FromSeconds(1),
                                              gfx::Tween::ZERO);
        set_enter_animation_type(DEFAULT);
        return;
      }
      if (window_state->IsMaximized())
        window_state->SetBoundsDirectCrossFade(bounds_in_parent);
      else if (window_state->IsSnapped())
        window_state->SetBoundsDirect(bounds_in_parent);
      else
        window_state->SetBoundsDirectAnimated(bounds_in_parent);
    }
  }
}
