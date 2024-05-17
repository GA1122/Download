void LayerTreeHost::NotifySwapPromiseMonitorsOfSetNeedsCommit() {
  std::set<SwapPromiseMonitor*>::iterator it = swap_promise_monitor_.begin();
  for (; it != swap_promise_monitor_.end(); it++)
    (*it)->OnSetNeedsCommitOnMain();
}
