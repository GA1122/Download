void RenderWidgetHostImpl::SetCursor(const CursorInfo& cursor_info) {
  WebCursor cursor;
  cursor.InitFromCursorInfo(cursor_info);
  SetCursor(cursor);
}
