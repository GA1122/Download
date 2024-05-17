void BrowserViewRenderer::DidInitializeCompositor(
    content::SynchronousCompositor* compositor) {
  TRACE_EVENT0("android_webview",
               "BrowserViewRenderer::DidInitializeCompositor");
  DCHECK(compositor);
  DCHECK(!compositor_);
  compositor_ = compositor;
  UpdateCompositorIsActive();
}
