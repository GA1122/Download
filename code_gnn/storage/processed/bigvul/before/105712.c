  bool MaybeLaunchInputMethodDaemon() {
    if (!initialized_successfully_)
      return false;

    if (!should_launch_ime_) {
      return false;
    }

#if !defined(TOUCH_UI)
    if (!candidate_window_controller_.get()) {
      candidate_window_controller_.reset(new CandidateWindowController);
      if (!candidate_window_controller_->Init()) {
        LOG(WARNING) << "Failed to initialize the candidate window controller";
      }
    }
#endif

    if (ibus_daemon_process_handle_ != base::kNullProcessHandle) {
      return false;   
    }

    const std::string ibus_daemon_command_line =
        StringPrintf("%s --panel=disable --cache=none --restart --replace",
                     kIBusDaemonPath);
    if (!LaunchInputMethodProcess(
            ibus_daemon_command_line, &ibus_daemon_process_handle_)) {
      LOG(ERROR) << "Failed to launch " << ibus_daemon_command_line;
      return false;
    }
    return true;
  }
