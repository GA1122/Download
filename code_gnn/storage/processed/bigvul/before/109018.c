void RenderViewImpl::PpapiPluginTextInputTypeChanged() {
  UpdateTextInputState(DO_NOT_SHOW_IME);
  if (renderer_accessibility_)
    renderer_accessibility_->FocusedNodeChanged(WebNode());
}
