  void FlushMonitoring(WaitableEvent* flush_complete_event) {
    TraceLog::GetInstance()->FlushButLeaveBufferIntact(
        base::Bind(&TraceEventTestFixture::OnTraceDataCollected,
                   base::Unretained(static_cast<TraceEventTestFixture*>(this)),
                   base::Unretained(flush_complete_event)));
  }
