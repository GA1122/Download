bool WebContentsImpl::OnUpdateDragCursor() {
  if (browser_plugin_embedder_)
    return browser_plugin_embedder_->OnUpdateDragCursor();
  return false;
}
