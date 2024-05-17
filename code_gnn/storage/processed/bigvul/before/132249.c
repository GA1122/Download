void RenderFrameImpl::OnSelectRange(const gfx::Point& base,
                                    const gfx::Point& extent) {
  Send(new InputHostMsg_SelectRange_ACK(GetRenderWidget()->routing_id()));

  base::AutoReset<bool> handling_select_range(&handling_select_range_, true);
  frame_->selectRange(base, extent);
}
