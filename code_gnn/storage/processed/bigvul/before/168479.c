void CoordinatorImpl::RequestGlobalMemoryDumpInternal(
    const QueuedRequest::Args& args,
    RequestGlobalMemoryDumpInternalCallback callback) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

  UMA_HISTOGRAM_COUNTS_1000("Memory.Experimental.Debug.GlobalDumpQueueLength",
                            queued_memory_dump_requests_.size());

  bool another_dump_is_queued = !queued_memory_dump_requests_.empty();

  if (another_dump_is_queued &&
      args.dump_type == MemoryDumpType::PERIODIC_INTERVAL) {
    for (const auto& request : queued_memory_dump_requests_) {
      if (request.args.level_of_detail == args.level_of_detail) {
        VLOG(1) << "RequestGlobalMemoryDump("
                << base::trace_event::MemoryDumpTypeToString(args.dump_type)
                << ") skipped because another dump request with the same "
                   "level of detail ("
                << base::trace_event::MemoryDumpLevelOfDetailToString(
                       args.level_of_detail)
                << ") is already in the queue";
        std::move(callback).Run(false  , 0  ,
                                nullptr  );
        return;
      }
    }
  }

  queued_memory_dump_requests_.emplace_back(args, ++next_dump_id_,
                                            std::move(callback));

  if (another_dump_is_queued)
    return;

  PerformNextQueuedGlobalMemoryDump();
}
