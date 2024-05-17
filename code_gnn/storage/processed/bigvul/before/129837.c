  void BeginSpecificTrace(const std::string& filter) {
    event_watch_notification_ = 0;
    TraceLog::GetInstance()->SetEnabled(TraceConfig(filter, ""),
                                        TraceLog::RECORDING_MODE);
  }
