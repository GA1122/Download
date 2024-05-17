   void StopInputMethodDaemon() {
     if (!initialized_successfully_)
       return;
 
      should_launch_ime_ = false;
      if (ibus_daemon_process_handle_ != base::kNullProcessHandle) {
        const base::ProcessId pid = base::GetProcId(ibus_daemon_process_handle_);
      if (!chromeos::StopInputMethodProcess(input_method_status_connection_)) {
//       if (!ibus_controller_->StopInputMethodProcess()) {
          LOG(ERROR) << "StopInputMethodProcess IPC failed. Sending SIGTERM to "
                     << "PID " << pid;
          base::KillProcess(ibus_daemon_process_handle_, -1, false  );
       }
       VLOG(1) << "ibus-daemon (PID=" << pid << ") is terminated";
       ibus_daemon_process_handle_ = base::kNullProcessHandle;
     }
   }