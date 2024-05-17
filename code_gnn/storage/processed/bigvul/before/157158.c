UrlIndex::UrlIndex(ResourceFetchContext* fetch_context, int block_shift)
    : fetch_context_(fetch_context),
      lru_(new MultiBuffer::GlobalLRU(base::ThreadTaskRunnerHandle::Get())),
      block_shift_(block_shift),
      memory_pressure_listener_(
          base::Bind(&UrlIndex::OnMemoryPressure, base::Unretained(this))) {}
