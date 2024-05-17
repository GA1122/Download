void CoordinatorImpl::OnQueuedRequestTimedOut(uint64_t dump_guid) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  QueuedRequest* request = GetCurrentRequest();


  if (!request || request->dump_guid != dump_guid)
    return;

  request->failed_memory_dump_count += request->pending_responses.size();
  request->pending_responses.clear();

  FinalizeGlobalMemoryDumpIfAllManagersReplied();
}
