void RenderViewImpl::hasTouchEventHandlers(bool has_handlers) {
  Send(new ViewHostMsg_HasTouchEventHandlers(routing_id_, has_handlers));
}
