  void CancelTrace() {
    WaitableEvent flush_complete_event(false, false);
    CancelTraceAsync(&flush_complete_event);
    flush_complete_event.Wait();
  }
