void TabletModeWindowState::UpdateWindowPosition(wm::WindowState* window_state,
                                                 bool animate) {
  gfx::Rect bounds_in_parent = GetBoundsInTabletMode(window_state);
  if (bounds_in_parent == window_state->window()->GetTargetBounds())
    return;

  if (animate)
    window_state->SetBoundsDirectAnimated(bounds_in_parent);
  else
    window_state->SetBoundsDirect(bounds_in_parent);
}
