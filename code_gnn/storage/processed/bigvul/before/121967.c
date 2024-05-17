gfx::Point AppListController::FindAnchorPoint(
    const gfx::Display& display,
    const gfx::Point& cursor) {
  const int kSnapOffset = 3;

  gfx::Rect bounds_rect(display.work_area());
  gfx::Rect taskbar_rect;
  if (GetTaskbarRect(&taskbar_rect))
    bounds_rect.Subtract(taskbar_rect);

  gfx::Size view_size(current_view_->GetPreferredSize());
  bounds_rect.Inset(view_size.width() / 2 + kSnapOffset,
                    view_size.height() / 2 + kSnapOffset);

  gfx::Point anchor = FindReferencePoint(display, cursor);
  anchor.SetToMax(bounds_rect.origin());
  anchor.SetToMin(bounds_rect.bottom_right());
  return anchor;
}
