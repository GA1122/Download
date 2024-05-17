void CoordinatorImpl::OnDumpProcessesForTracing(
    uint64_t dump_guid,
    std::vector<mojom::SharedBufferWithSizePtr> buffers) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  QueuedRequest* request = GetCurrentRequest();
  if (!request || request->dump_guid != dump_guid) {
    return;
  }

  request->heap_dump_in_progress = false;

  for (auto& buffer_ptr : buffers) {
    mojo::ScopedSharedBufferHandle& buffer = buffer_ptr->buffer;
    uint32_t size = buffer_ptr->size;

    if (!buffer->is_valid())
      continue;

    mojo::ScopedSharedBufferMapping mapping = buffer->Map(size);
    if (!mapping) {
      DLOG(ERROR) << "Failed to map buffer";
      continue;
    }

    const char* char_buffer = static_cast<const char*>(mapping.get());
    std::string json(char_buffer, char_buffer + size);

    const int kTraceEventNumArgs = 1;
    const char* const kTraceEventArgNames[] = {"dumps"};
    const unsigned char kTraceEventArgTypes[] = {TRACE_VALUE_TYPE_CONVERTABLE};
    std::unique_ptr<base::trace_event::ConvertableToTraceFormat> wrapper(
        new StringWrapper(std::move(json)));

    TRACE_EVENT_API_ADD_TRACE_EVENT_WITH_PROCESS_ID(
        TRACE_EVENT_PHASE_MEMORY_DUMP,
        base::trace_event::TraceLog::GetCategoryGroupEnabled(
            base::trace_event::MemoryDumpManager::kTraceCategory),
        "periodic_interval", trace_event_internal::kGlobalScope, dump_guid,
        buffer_ptr->pid, kTraceEventNumArgs, kTraceEventArgNames,
        kTraceEventArgTypes, nullptr  , &wrapper,
        TRACE_EVENT_FLAG_HAS_ID);
  }

  FinalizeGlobalMemoryDumpIfAllManagersReplied();
}
