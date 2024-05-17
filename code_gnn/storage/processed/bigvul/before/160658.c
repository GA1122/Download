void RenderFrameImpl::OnExtendSelectionAndDelete(int before, int after) {
  ImeEventGuard guard(GetRenderWidget());
  frame_->ExtendSelectionAndDelete(before, after);
}
