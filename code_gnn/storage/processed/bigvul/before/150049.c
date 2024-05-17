void LayerTreeHostImpl::SetNeedsMutate() {
  TRACE_EVENT0("compositor-worker", "LayerTreeHostImpl::SetNeedsMutate");
  client_->SetNeedsOneBeginImplFrameOnImplThread();
}
