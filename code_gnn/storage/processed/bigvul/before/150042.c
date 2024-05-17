void LayerTreeHostImpl::SetLayerTreeMutator(
    std::unique_ptr<LayerTreeMutator> mutator) {
  if (mutator == mutator_)
    return;
  TRACE_EVENT0(TRACE_DISABLED_BY_DEFAULT("compositor-worker"),
               "LayerTreeHostImpl::SetLayerTreeMutator");
  mutator_ = std::move(mutator);
  mutator_->SetClient(this);
}
