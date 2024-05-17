void LayerTreeHost::SetNeedsUpdateLayers() {
  proxy_->SetNeedsUpdateLayers();
  NotifySwapPromiseMonitorsOfSetNeedsCommit();
}
