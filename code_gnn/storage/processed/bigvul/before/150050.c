void LayerTreeHostImpl::SetNeedsOneBeginImplFrame() {
  NotifySwapPromiseMonitorsOfSetNeedsRedraw();
  client_->SetNeedsOneBeginImplFrameOnImplThread();
}
