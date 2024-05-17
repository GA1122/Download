void RenderWidgetHostImpl::OnRequestSetBounds(const gfx::Rect& bounds) {
  if (view_) {
    view_->SetBounds(bounds);
    Send(new ViewMsg_SetBounds_ACK(routing_id_));
  }
}
