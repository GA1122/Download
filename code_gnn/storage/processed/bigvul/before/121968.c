gfx::Point FindReferencePoint(const gfx::Display& display,
                              const gfx::Point& cursor) {
  const int kSnapDistance = 50;

  gfx::Rect taskbar_rect;
  if (!GetTaskbarRect(&taskbar_rect) ||
      (display.work_area() == display.bounds() &&
          !display.work_area().Contains(taskbar_rect))) {
    return display.work_area().bottom_left();
  }

  const gfx::Rect& screen_rect = display.bounds();
  if (taskbar_rect.width() == screen_rect.width()) {
    if (taskbar_rect.bottom() == screen_rect.bottom()) {
      if (taskbar_rect.y() - cursor.y() > kSnapDistance)
        return gfx::Point(screen_rect.x(), taskbar_rect.y());

      return gfx::Point(cursor.x(), taskbar_rect.y());
    }

    if (cursor.y() - taskbar_rect.bottom() > kSnapDistance)
      return gfx::Point(screen_rect.x(), taskbar_rect.bottom());

    return gfx::Point(cursor.x(), taskbar_rect.bottom());
  }

  if (taskbar_rect.x() == screen_rect.x()) {
    if (cursor.x() - taskbar_rect.right() > kSnapDistance)
      return gfx::Point(taskbar_rect.right(), screen_rect.y());

    return gfx::Point(taskbar_rect.right(), cursor.y());
  }

  if (taskbar_rect.x() - cursor.x() > kSnapDistance)
    return gfx::Point(taskbar_rect.x(), screen_rect.y());

  return gfx::Point(taskbar_rect.x(), cursor.y());
}
