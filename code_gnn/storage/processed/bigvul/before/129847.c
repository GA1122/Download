  void EndTraceAndFlushAsync(WaitableEvent* flush_complete_event) {
    TraceLog::GetInstance()->SetDisabled();
    TraceLog::GetInstance()->Flush(
        base::Bind(&TraceEventTestFixture::OnTraceDataCollected,
                   base::Unretained(static_cast<TraceEventTestFixture*>(this)),
                   base::Unretained(flush_complete_event)));
  }
