void RenderViewImpl::willInsertBody(WebKit::WebFrame* frame) {
  if (!frame->parent()) {
    Send(new ViewHostMsg_WillInsertBody(routing_id()));
  }
}
