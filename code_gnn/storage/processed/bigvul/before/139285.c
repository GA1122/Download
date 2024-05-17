  void StopTracing() {
    CHECK(message_loop_runner_.get() == NULL)
        << "Calling StopTracing more than once";
    trace_log_->SetDisabled();
    message_loop_runner_ = new MessageLoopRunner;
    trace_log_->Flush(base::Bind(
        &WebRtcGetUserMediaBrowserTest::OnTraceDataCollected,
        base::Unretained(this)));
    message_loop_runner_->Run();
  }
