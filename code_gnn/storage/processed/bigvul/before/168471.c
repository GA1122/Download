void CoordinatorImpl::FinalizeVmRegionDumpIfAllManagersReplied(
    uint64_t dump_guid) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

  auto it = in_progress_vm_region_requests_.find(dump_guid);
  if (it == in_progress_vm_region_requests_.end())
    return;

  if (!it->second->pending_responses.empty())
    return;

  QueuedRequestDispatcher::VmRegions results =
      QueuedRequestDispatcher::FinalizeVmRegionRequest(it->second.get());
  std::move(it->second->callback).Run(std::move(results));
  in_progress_vm_region_requests_.erase(it);
}
