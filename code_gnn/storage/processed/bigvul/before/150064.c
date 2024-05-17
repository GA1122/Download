bool LayerTreeHostImpl::SupportsImplScrolling() const {
  return task_runner_provider_->HasImplThread();
}
