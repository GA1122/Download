void ChromeClientImpl::RegisterViewportLayers() const {
  if (web_view_->RootGraphicsLayer() && web_view_->LayerTreeView())
    web_view_->RegisterViewportLayersWithCompositor();
}
