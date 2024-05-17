void NotifyProcessKilled(const ChildProcessData& data, int exit_code) {
  for (auto& observer : g_browser_child_process_observers.Get())
    observer.BrowserChildProcessKilled(data, exit_code);
}
