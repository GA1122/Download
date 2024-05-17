bool LayerTreeHostImpl::CommitToActiveTree() const {
  return !task_runner_provider_->HasImplThread();
}
