void RenderViewHostImpl::OnRequestMove(const gfx::Rect& pos) {
  if (!is_swapped_out_)
    delegate_->RequestMove(pos);
  Send(new ViewMsg_Move_ACK(GetRoutingID()));
}
