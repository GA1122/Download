void RenderFrameImpl::DidChangeOpener(blink::WebFrame* opener) {
  DCHECK(!opener || opener->IsWebLocalFrame());

  int opener_routing_id =
      opener ? RenderFrameImpl::FromWebFrame(opener->ToWebLocalFrame())
                   ->GetRoutingID()
             : MSG_ROUTING_NONE;
  Send(new FrameHostMsg_DidChangeOpener(routing_id_, opener_routing_id));
}
