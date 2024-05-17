bool BrowserViewRenderer::OnDrawHardware() {
  TRACE_EVENT0("android_webview", "BrowserViewRenderer::OnDrawHardware");

  shared_renderer_state_.InitializeHardwareDrawIfNeededOnUI();

  if (!CanOnDraw()) {
    return false;
  }

  shared_renderer_state_.SetScrollOffsetOnUI(last_on_draw_scroll_offset_);
  hardware_enabled_ = true;

  return CompositeHw();
}
