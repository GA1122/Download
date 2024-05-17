  void EndTraceAndFlush() {
    num_flush_callbacks_ = 0;
    WaitableEvent flush_complete_event(false, false);
    EndTraceAndFlushAsync(&flush_complete_event);
    flush_complete_event.Wait();
  }
