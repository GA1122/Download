void RenderFrameImpl::OnSelectRange(const gfx::Point& base,
                                    const gfx::Point& extent) {
  Send(new InputHostMsg_SelectRange_ACK(GetRenderWidget()->routing_id()));

  AutoResetMember<bool> handling_select_range(
      this, &RenderFrameImpl::handling_select_range_, true);
  frame_->SelectRange(render_view_->ConvertWindowPointToViewport(base),
                      render_view_->ConvertWindowPointToViewport(extent));
}
