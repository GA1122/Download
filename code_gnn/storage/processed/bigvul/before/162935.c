  void RequestChromeDump(MemoryDumpType dump_type,
                         MemoryDumpLevelOfDetail level_of_detail) {
    uint64_t req_guid = ++guid_counter_;
    MemoryDumpRequestArgs request_args{req_guid, dump_type, level_of_detail};
    ClientProcessImpl::RequestChromeMemoryDumpCallback callback = base::Bind(
        [](bool success, uint64_t dump_guid,
           std::unique_ptr<base::trace_event::ProcessMemoryDump> result) {});
    client_process_->RequestChromeMemoryDump(request_args, callback);
  }
