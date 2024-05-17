  gfx::Point CalculateDragPoint(const ash::WindowResizer& resizer,
                                int delta_x,
                                int delta_y) {
    gfx::Point location = resizer.GetInitialLocation();
    location.set_x(location.x() + delta_x);
    location.set_y(location.y() + delta_y);
    return location;
  }
