void PageInfoWindowView::CalculateWindowBounds(gfx::Rect* bounds) {
  const int kDefaultOffset = 15;

#if defined(OS_WIN)
  gfx::Rect monitor_bounds(win_util::GetMonitorBoundsForRect(*bounds));
  if (monitor_bounds.IsEmpty())
    return;
#else
  gfx::Rect monitor_bounds(0, 0, 1024, 768);
#endif

  gfx::Rect adjusted_bounds = bounds->AdjustToFit(monitor_bounds);
  if (adjusted_bounds != *bounds) {
    *bounds = adjusted_bounds;
    return;
  }

  int x_offset, y_offset;
  if (bounds->right() + kDefaultOffset >= monitor_bounds.right() &&
      abs(monitor_bounds.x() - bounds->x()) >= kDefaultOffset) {
    x_offset = -kDefaultOffset;
  } else {
    x_offset = kDefaultOffset;
  }

  if (bounds->bottom() + kDefaultOffset >= monitor_bounds.bottom() &&
      abs(monitor_bounds.y() - bounds->y()) >= kDefaultOffset) {
    y_offset = -kDefaultOffset;
  } else {
    y_offset = kDefaultOffset;
  }

  bounds->Offset(x_offset, y_offset);
}
