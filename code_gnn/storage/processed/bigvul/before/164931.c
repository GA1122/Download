ResourceLoader* ResourceDispatcherHostImpl::GetLoader(
    const GlobalRequestID& id) const {
  DCHECK(io_thread_task_runner_->BelongsToCurrentThread());

  auto i = pending_loaders_.find(id);
  if (i == pending_loaders_.end())
    return nullptr;

  return i->second.get();
}
