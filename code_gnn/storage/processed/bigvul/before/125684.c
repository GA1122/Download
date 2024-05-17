void RenderViewHostImpl::OnShowFullscreenWidget(int route_id) {
  if (!is_swapped_out_)
    delegate_->ShowCreatedFullscreenWidget(route_id);
  Send(new ViewMsg_Move_ACK(route_id));
}
