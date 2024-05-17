void RenderViewHostImpl::DragTargetDrop(
    const gfx::Point& client_pt,
    const gfx::Point& screen_pt,
    int key_modifiers) {
  Send(new DragMsg_TargetDrop(GetRoutingID(), client_pt, screen_pt,
                              key_modifiers));
}
