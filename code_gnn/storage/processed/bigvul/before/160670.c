void RenderFrameImpl::OnMoveRangeSelectionExtent(const gfx::Point& point) {
  Send(new InputHostMsg_MoveRangeSelectionExtent_ACK(
      GetRenderWidget()->routing_id()));

  AutoResetMember<bool> handling_select_range(
      this, &RenderFrameImpl::handling_select_range_, true);
  frame_->MoveRangeSelectionExtent(
      render_view_->ConvertWindowPointToViewport(point));
}
