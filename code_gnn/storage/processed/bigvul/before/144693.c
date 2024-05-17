void WebContentsImpl::SystemDragEnded() {
  if (GetRenderViewHost())
    GetRenderViewHost()->DragSourceSystemDragEnded();
  if (browser_plugin_embedder_.get())
    browser_plugin_embedder_->SystemDragEnded();
}
