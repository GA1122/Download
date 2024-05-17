void ChromeClientImpl::BeginLifecycleUpdates() {
  if (WebLayerTreeView* tree_view = web_view_->LayerTreeView()) {
    tree_view->SetDeferCommits(false);
    tree_view->SetNeedsBeginFrame();
  }
}
