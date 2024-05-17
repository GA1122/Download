void RenderViewImpl::numberOfTouchEventHandlersChanged(unsigned num_handlers) {
  Send(new ViewHostMsg_DidChangeNumTouchEvents(routing_id_, num_handlers));
}
