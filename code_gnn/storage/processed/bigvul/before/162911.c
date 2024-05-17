void CoordinatorImpl::UnregisterClientProcess(
    mojom::ClientProcess* client_process) {
  QueuedRequest* request = GetCurrentRequest();
  if (request != nullptr) {
    auto it = request->pending_responses.begin();
    while (it != request->pending_responses.end()) {
      std::set<QueuedRequest::PendingResponse>::iterator current = it++;
      if (current->client != client_process)
        continue;
      RemovePendingResponse(client_process, current->type);
      request->failed_memory_dump_count++;
    }
    FinalizeGlobalMemoryDumpIfAllManagersReplied();
  }
  size_t num_deleted = clients_.erase(client_process);
  DCHECK(num_deleted == 1);
}
