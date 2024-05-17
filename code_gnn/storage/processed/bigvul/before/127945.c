void BrowserViewRenderer::OnComputeScroll(base::TimeTicks animation_time) {
  if (!compositor_)
    return;
  TRACE_EVENT0("android_webview", "BrowserViewRenderer::OnComputeScroll");
  compositor_->OnComputeScroll(animation_time);
}
