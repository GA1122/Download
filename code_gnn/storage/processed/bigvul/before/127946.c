void BrowserViewRenderer::OnDetachedFromWindow() {
  TRACE_EVENT0("android_webview", "BrowserViewRenderer::OnDetachedFromWindow");
  shared_renderer_state_.ReleaseHardwareDrawIfNeededOnUI();
  attached_to_window_ = false;
  DCHECK(!hardware_enabled_);
  UpdateCompositorIsActive();
}
