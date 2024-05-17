void BrowserViewRenderer::PostInvalidate() {
  TRACE_EVENT_INSTANT0("android_webview", "BrowserViewRenderer::PostInvalidate",
                       TRACE_EVENT_SCOPE_THREAD);
  PostInvalidateWithFallback();
}
