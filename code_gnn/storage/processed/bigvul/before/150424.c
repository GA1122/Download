  void OnBoundsChangeEvent(ClientControlledShellSurface* shell_surface,
                           ash::WindowStateType current_state,
                           ash::WindowStateType requested_state,
                           int64_t display_id,
                           const gfx::Rect& bounds_in_screen,
                           bool is_resize,
                           int bounds_change) {
    bounds_change_count_++;

    display::Display target_display;
    const display::Screen* screen = display::Screen::GetScreen();

    ASSERT_TRUE(screen->GetDisplayWithDisplayId(display_id, &target_display));
    gfx::Rect bounds_in_display(bounds_in_screen);
    bounds_in_display.Offset(-target_display.bounds().OffsetFromOrigin());

    requested_bounds_.push_back(bounds_in_display);
    requested_display_ids_.push_back(display_id);
  }
