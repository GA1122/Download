  bool RequestChromeDumpAndWait(
      MemoryDumpType dump_type,
      MemoryDumpLevelOfDetail level_of_detail,
      std::unique_ptr<base::trace_event::ProcessMemoryDump>* result = nullptr) {
    base::RunLoop run_loop;
    bool success = false;
    uint64_t req_guid = ++guid_counter_;
    MemoryDumpRequestArgs request_args{req_guid, dump_type, level_of_detail};
    ClientProcessImpl::RequestChromeMemoryDumpCallback callback = base::Bind(
        [](bool* curried_success, base::Closure curried_quit_closure,
           std::unique_ptr<base::trace_event::ProcessMemoryDump>*
               curried_result,
           uint64_t curried_expected_guid, bool success, uint64_t dump_guid,
           std::unique_ptr<base::trace_event::ProcessMemoryDump> result) {
          EXPECT_EQ(curried_expected_guid, dump_guid);
          *curried_success = success;
          if (curried_result)
            *curried_result = std::move(result);
          curried_quit_closure.Run();
        },
        &success, run_loop.QuitClosure(), result, req_guid);
    client_process_->RequestChromeMemoryDump(request_args, callback);
    run_loop.Run();
    return success;
  }
