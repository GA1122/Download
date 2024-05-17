void RenderFrameImpl::OnDeleteSurroundingText(int before, int after) {
  ImeEventGuard guard(GetRenderWidget());
  frame_->DeleteSurroundingText(before, after);
}
