void ClientControlledShellSurface::SetWidgetBounds(const gfx::Rect& bounds) {
  const auto* screen = display::Screen::GetScreen();
  aura::Window* window = widget_->GetNativeWindow();
  display::Display current_display = screen->GetDisplayNearestWindow(window);

  bool is_display_move_pending = false;
  display::Display target_display = current_display;

  display::Display display;
  if (screen->GetDisplayWithDisplayId(display_id_, &display)) {
    bool is_display_stale = display_id_ != current_display.id();

    if (preserve_widget_bounds_ && is_display_stale)
      return;

    is_display_move_pending =
        window->GetProperty(ash::kLockedToRootKey) && is_display_stale;

    if (!is_display_move_pending)
      target_display = display;

    preserve_widget_bounds_ = is_display_move_pending;
  } else {
    preserve_widget_bounds_ = false;
  }

  gfx::Rect adjusted_bounds = bounds;
  if (!is_display_move_pending) {
    ash::wm::ClientControlledState::AdjustBoundsForMinimumWindowVisibility(
        target_display.bounds(), &adjusted_bounds);
  }

  if (adjusted_bounds == widget_->GetWindowBoundsInScreen() &&
      target_display.id() == current_display.id()) {
    return;
  }

  bool set_bounds_locally =
      GetWindowState()->is_dragged() && !is_display_move_pending;

  if (set_bounds_locally || client_controlled_state_->set_bounds_locally()) {
    gfx::Point origin = bounds.origin();
    wm::ConvertPointFromScreen(window->parent(), &origin);

    {
      ScopedSetBoundsLocally scoped_set_bounds(this);
      window->SetBounds(gfx::Rect(origin, adjusted_bounds.size()));
    }
    UpdateSurfaceBounds();
    return;
  }

  {
    ScopedSetBoundsLocally scoped_set_bounds(this);
    window->SetBoundsInScreen(adjusted_bounds, target_display);
  }

  if (bounds != adjusted_bounds || is_display_move_pending) {
    auto state_type = GetWindowState()->GetStateType();
    OnBoundsChangeEvent(state_type, state_type, target_display.id(),
                        adjusted_bounds, 0);
  }

  UpdateSurfaceBounds();
}
