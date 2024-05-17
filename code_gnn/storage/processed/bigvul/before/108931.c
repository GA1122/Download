void RenderViewImpl::OnDragTargetDrop(const gfx::Point& client_point,
                                      const gfx::Point& screen_point,
                                      int key_modifiers) {
  webview()->dragTargetDrop(client_point, screen_point, key_modifiers);

  Send(new DragHostMsg_TargetDrop_ACK(routing_id_));
}
