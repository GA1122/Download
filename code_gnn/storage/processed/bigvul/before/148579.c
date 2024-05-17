void WebContentsImpl::SystemDragEnded(RenderWidgetHost* source_rwh) {
  if (source_rwh)
    source_rwh->DragSourceSystemDragEnded();
  if (browser_plugin_embedder_.get())
    browser_plugin_embedder_->SystemDragEnded();
}
