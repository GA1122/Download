void RenderFrameImpl::OnUpdateOpener(int opener_routing_id) {
  WebFrame* opener = ResolveOpener(opener_routing_id);
  frame_->SetOpener(opener);
}
