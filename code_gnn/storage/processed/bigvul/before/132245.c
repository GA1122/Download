void RenderFrameImpl::OnMoveRangeSelectionExtent(const gfx::Point& point) {
  Send(new InputHostMsg_MoveRangeSelectionExtent_ACK(
      GetRenderWidget()->routing_id()));

  base::AutoReset<bool> handling_select_range(&handling_select_range_, true);
  frame_->moveRangeSelectionExtent(point);
}
