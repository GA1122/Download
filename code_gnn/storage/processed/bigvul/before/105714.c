  static void OnImeShutdown(GPid pid,
                            gint status,
                            InputMethodLibraryImpl* library) {
    if (library->ibus_daemon_process_handle_ != base::kNullProcessHandle &&
        base::GetProcId(library->ibus_daemon_process_handle_) == pid) {
      library->ibus_daemon_process_handle_ = base::kNullProcessHandle;
    }

    library->MaybeLaunchInputMethodDaemon();
  }
