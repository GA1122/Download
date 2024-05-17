void LayerTreeHost::SetNeedsAnimate() {
  proxy_->SetNeedsAnimate();
  NotifySwapPromiseMonitorsOfSetNeedsCommit();
}
