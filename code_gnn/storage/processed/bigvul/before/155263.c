  bool Initialize(bool dump_at_crash) {
    request_dump_ = dump_at_crash ? 1 : 0;

    SetSanitizationInfo(crash_reporter::GetCrashReporterClient(),
                        &sanitization_);
    server_fd_ = base::GlobalDescriptors::GetInstance()->Get(
        service_manager::kCrashDumpSignal);

    return Signals::InstallCrashHandlers(HandleCrash, 0, &old_actions_);
  }
