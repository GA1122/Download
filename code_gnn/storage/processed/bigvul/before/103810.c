void RenderView::OnDragTargetDragEnter(const WebDropData& drop_data,
                                       const gfx::Point& client_point,
                                       const gfx::Point& screen_point,
                                       WebDragOperationsMask ops) {
  WebDragOperation operation = webview()->dragTargetDragEnter(
      drop_data.ToDragData(),
      client_point,
      screen_point,
      ops);

  Send(new DragHostMsg_UpdateDragCursor(routing_id_, operation));
}
