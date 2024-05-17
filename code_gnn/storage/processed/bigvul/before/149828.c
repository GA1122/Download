void LayerTreeHost::SetAnimationEvents(
    std::unique_ptr<MutatorEvents> events) {
  DCHECK(task_runner_provider_->IsMainThread());
  mutator_host_->SetAnimationEvents(std::move(events));
}
