void RenderViewImpl::PpapiPluginFocusChanged() {
  UpdateTextInputState(DO_NOT_SHOW_IME);
  UpdateSelectionBounds();
}
