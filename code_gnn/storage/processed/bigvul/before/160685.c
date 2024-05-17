void RenderFrameImpl::OnSetEditableSelectionOffsets(int start, int end) {
  AutoResetMember<bool> handling_select_range(
      this, &RenderFrameImpl::handling_select_range_, true);
  ImeEventGuard guard(GetRenderWidget());
  frame_->SetEditableSelectionOffsets(start, end);
}
