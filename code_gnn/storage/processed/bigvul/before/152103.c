void RenderFrameHostImpl::UpdateAccessibilityMode() {
  Send(new FrameMsg_SetAccessibilityMode(routing_id_,
                                         delegate_->GetAccessibilityMode()));
}
