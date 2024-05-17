void CoordinatorImpl::OnHeapDumpTimeOut(uint64_t dump_guid) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  QueuedRequest* request = GetCurrentRequest();


  if (!request || request->dump_guid != dump_guid)
    return;

  if (request->heap_dump_in_progress) {
    request->heap_dump_in_progress = false;
    FinalizeGlobalMemoryDumpIfAllManagersReplied();
  }
}
