void RenderViewImpl::OnDragTargetDragEnter(const WebDropData& drop_data,
                                           const gfx::Point& client_point,
                                           const gfx::Point& screen_point,
                                           WebDragOperationsMask ops,
                                           int key_modifiers) {
  WebDragOperation operation = webview()->dragTargetDragEnter(
      drop_data.ToDragData(),
      client_point,
      screen_point,
      ops,
      key_modifiers);

  Send(new DragHostMsg_UpdateDragCursor(routing_id_, operation));
}
