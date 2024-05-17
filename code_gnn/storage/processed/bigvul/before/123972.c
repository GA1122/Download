void RenderViewImpl::didDisownOpener(WebKit::WebFrame* frame) {
  CHECK(!frame->parent());

  if (is_swapped_out_)
    return;

  Send(new ViewHostMsg_DidDisownOpener(routing_id_));
}
