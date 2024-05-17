gfx::NativeCursor ResizeGripper::GetCursorForPoint(Event::EventType event_type,
                                                   const gfx::Point& p) {
  if (!enabled_)
    return NULL;
#if defined(OS_WIN)
  if (!g_resize_cursor)
    g_resize_cursor = LoadCursor(NULL, IDC_SIZEWE);
  return g_resize_cursor;
#elif defined(OS_LINUX)
  return gdk_cursor_new(GDK_SB_H_DOUBLE_ARROW);
 #endif
 }
