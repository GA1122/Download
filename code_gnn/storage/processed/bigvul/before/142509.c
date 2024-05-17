  gfx::Rect GetVisibleShelfWidgetBoundsInScreen() {
    gfx::Rect bounds = GetShelfWidget()->GetWindowBoundsInScreen();
    bounds.Intersect(
        display::Screen::GetScreen()->GetPrimaryDisplay().bounds());
    return bounds;
  }
