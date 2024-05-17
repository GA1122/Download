void ProfilingProcessHost::OnDumpProcessesForTracingCallback(
    uint64_t guid,
    std::vector<profiling::mojom::SharedBufferWithSizePtr> buffers) {
  for (auto& buffer_ptr : buffers) {
    mojo::ScopedSharedBufferHandle& buffer = buffer_ptr->buffer;
    uint32_t size = buffer_ptr->size;

    if (!buffer->is_valid())
      return;

    mojo::ScopedSharedBufferMapping mapping = buffer->Map(size);
    if (!mapping) {
      DLOG(ERROR) << "Failed to map buffer";
      return;
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
        "periodic_interval", trace_event_internal::kGlobalScope, guid,
        buffer_ptr->pid, kTraceEventNumArgs, kTraceEventArgNames,
        kTraceEventArgTypes, nullptr  , &wrapper,
        TRACE_EVENT_FLAG_HAS_ID);
  }
  if (dump_process_for_tracing_callback_) {
    std::move(dump_process_for_tracing_callback_).Run();
    dump_process_for_tracing_callback_.Reset();
  }
}
