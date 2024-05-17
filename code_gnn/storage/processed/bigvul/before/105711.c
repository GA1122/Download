  bool LaunchInputMethodProcess(const std::string& command_line,
                                base::ProcessHandle* process_handle) {
    std::vector<std::string> argv;
    base::file_handle_mapping_vector fds_to_remap;
    base::ProcessHandle handle = base::kNullProcessHandle;

    base::SplitString(command_line, ' ', &argv);
    const bool result = base::LaunchApp(argv,
                                        fds_to_remap,   
                                        false,   
                                        &handle);
    if (!result) {
      LOG(ERROR) << "Could not launch: " << command_line;
      return false;
    }

    const base::ProcessId pid = base::GetProcId(handle);
    g_child_watch_add(pid,
                      reinterpret_cast<GChildWatchFunc>(OnImeShutdown),
                      this);

    *process_handle = handle;
    VLOG(1) << command_line << " (PID=" << pid << ") is started";
    return  true;
  }
