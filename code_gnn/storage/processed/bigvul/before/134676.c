void BrowserViewRenderer::FallbackTickFired() {
  TRACE_EVENT0("android_webview", "BrowserViewRenderer::FallbackTickFired");
  DCHECK(fallback_tick_pending_);
  fallback_tick_fired_.Cancel();
  fallback_tick_pending_ = false;
  if (compositor_) {
    if (hardware_enabled_) {
      CompositeHw();
    } else {
      ForceFakeCompositeSW();
    }
  }
}
