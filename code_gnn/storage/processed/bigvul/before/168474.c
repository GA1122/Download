void CoordinatorImpl::OnOSMemoryDumpForVMRegions(uint64_t dump_guid,
                                                 mojom::ClientProcess* client,
                                                 bool success,
                                                 OSMemDumpMap os_dumps) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  auto request_it = in_progress_vm_region_requests_.find(dump_guid);
  DCHECK(request_it != in_progress_vm_region_requests_.end());

  QueuedVmRegionRequest* request = request_it->second.get();
  auto it = request->pending_responses.find(client);
  DCHECK(it != request->pending_responses.end());
  request->pending_responses.erase(it);
  request->responses[client].os_dumps = std::move(os_dumps);

  FinalizeVmRegionDumpIfAllManagersReplied(request->dump_guid);
}
