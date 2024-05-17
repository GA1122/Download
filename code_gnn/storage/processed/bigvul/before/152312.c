void RenderFrameImpl::FocusedNodeChanged(const WebNode& node) {
  has_scrolled_focused_editable_node_into_rect_ = false;
  bool is_editable = false;
  gfx::Rect node_bounds;
  if (!node.IsNull() && node.IsElementNode()) {
    WebElement element = const_cast<WebNode&>(node).To<WebElement>();
    blink::WebRect rect = element.BoundsInViewport();
    GetLocalRootRenderWidget()->ConvertViewportToWindow(&rect);
    is_editable = element.IsEditable();
    node_bounds = gfx::Rect(rect);
  }
  Send(new FrameHostMsg_FocusedNodeChanged(routing_id_, is_editable,
                                           node_bounds));
  GetLocalRootRenderWidget()->ClearTextInputState();

  for (auto& observer : observers_)
    observer.FocusedNodeChanged(node);
}
