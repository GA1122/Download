void RenderFrameImpl::OnDeleteSurroundingTextInCodePoints(int before,
                                                          int after) {
  ImeEventGuard guard(GetRenderWidget());
  frame_->DeleteSurroundingTextInCodePoints(before, after);
}
