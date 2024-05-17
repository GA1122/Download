void LayerTreeHostImpl::SetTreeActivationCallback(
    const base::Closure& callback) {
  DCHECK(task_runner_provider_->IsImplThread());
  tree_activation_callback_ = callback;
}
