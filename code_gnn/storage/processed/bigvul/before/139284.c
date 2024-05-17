  void StartTracing() {
    CHECK(trace_log_ == NULL) << "Can only can start tracing once";
    trace_log_ = base::trace_event::TraceLog::GetInstance();
    base::trace_event::TraceOptions trace_options(
        base::trace_event::RECORD_UNTIL_FULL);
    trace_options.enable_sampling = true;
    trace_log_->SetEnabled(base::trace_event::CategoryFilter("video"),
                           base::trace_event::TraceLog::RECORDING_MODE,
                           trace_options);
    EXPECT_EQ(trace_log_->GetNumTracesRecorded(), 1);
  }
