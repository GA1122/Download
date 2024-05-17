  void FlushMonitoring() {
    WaitableEvent flush_complete_event(false, false);
    FlushMonitoring(&flush_complete_event);
    flush_complete_event.Wait();
  }
