void ChromeClientImpl::SetCursor(const Cursor& cursor,
                                 LocalFrame* local_frame) {
  last_set_mouse_cursor_for_testing_ = cursor;
  SetCursor(WebCursorInfo(cursor), local_frame);
}
