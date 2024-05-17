void RenderFrameImpl::SimulateImeFinishComposingText(bool keep_selection) {
  render_view_->OnImeFinishComposingText(keep_selection);
}
