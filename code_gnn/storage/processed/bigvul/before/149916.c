LayerTreeHostImpl::CreateLatencyInfoSwapPromiseMonitor(
    ui::LatencyInfo* latency) {
  return base::WrapUnique(
      new LatencyInfoSwapPromiseMonitor(latency, NULL, this));
}
