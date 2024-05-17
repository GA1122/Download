void RenderWidgetHostImpl::OnRequestMove(const gfx::Rect& pos) {
  if (view_) {
    view_->SetBounds(pos);
    Send(new ViewMsg_Move_ACK(routing_id_));
  }
}
