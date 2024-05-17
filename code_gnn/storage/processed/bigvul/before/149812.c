void LayerTreeHost::LayoutAndUpdateLayers() {
  DCHECK(IsSingleThreaded());
  DCHECK(!settings_.single_thread_proxy_scheduler);
  RequestMainFrameUpdate();
  UpdateLayers();
}
