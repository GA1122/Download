void CoordinatorImpl::OnChromeMemoryDumpResponse(
    mojom::ClientProcess* client,
    bool success,
    uint64_t dump_guid,
    std::unique_ptr<base::trace_event::ProcessMemoryDump> chrome_memory_dump) {
  using ResponseType = QueuedRequest::PendingResponse::Type;
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  QueuedRequest* request = GetCurrentRequest();
  if (request == nullptr || request->dump_guid != dump_guid) {
    return;
  }

  RemovePendingResponse(client, ResponseType::kChromeDump);

  if (!clients_.count(client)) {
    VLOG(1) << "Received a memory dump response from an unregistered client";
    return;
  }
  auto* response = &request->responses[client];
  response->chrome_dump = std::move(chrome_memory_dump);

  if (!success) {
    request->failed_memory_dump_count++;
    VLOG(1) << "RequestGlobalMemoryDump() FAIL: NACK from client process";
  }

  FinalizeGlobalMemoryDumpIfAllManagersReplied();
}
