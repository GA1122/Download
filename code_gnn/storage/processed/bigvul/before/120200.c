void LayerTreeHost::AcquireLayerTextures() {
  DCHECK(proxy_->IsMainThread());
  proxy_->AcquireLayerTextures();
}
