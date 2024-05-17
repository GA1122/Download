void BrowserViewRenderer::CancelFallbackTick() {
  post_fallback_tick_.Cancel();
  fallback_tick_fired_.Cancel();
  fallback_tick_pending_ = false;
}
