bool LayerTreeHostImpl::Mutate(base::TimeTicks monotonic_time) {
  if (!mutator_)
    return false;
  TRACE_EVENT0("compositor-worker", "LayerTreeHostImpl::Mutate");
  if (mutator_->Mutate(monotonic_time, active_tree()))
    client_->SetNeedsOneBeginImplFrameOnImplThread();
  return true;
}
