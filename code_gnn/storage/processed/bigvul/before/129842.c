  void CancelTraceAsync(WaitableEvent* flush_complete_event) {
    TraceLog::GetInstance()->CancelTracing(
        base::Bind(&TraceEventTestFixture::OnTraceDataCollected,
                   base::Unretained(static_cast<TraceEventTestFixture*>(this)),
                   base::Unretained(flush_complete_event)));
  }
