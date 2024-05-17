void RenderFrameImpl::SimulateImeFinishComposingText(bool keep_selection) {
  GetMainFrameRenderWidget()->OnImeFinishComposingText(keep_selection);
}
