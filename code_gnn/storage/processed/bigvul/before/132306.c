void RenderFrameImpl::didDisownOpener(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  if (is_swapped_out_ || frame->parent())
    return;

  Send(new FrameHostMsg_DidDisownOpener(routing_id_));
}
