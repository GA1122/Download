void WebPluginDelegateStub::OnHandleInputEvent(
    const WebKit::WebInputEvent *event,
    bool* handled,
    WebCursor* cursor) {
  WebCursorInfo cursor_info;
  *handled = delegate_->HandleInputEvent(*event, &cursor_info);
  cursor->InitFromCursorInfo(cursor_info);
}
