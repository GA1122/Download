  void ResetVLogInitialized() {
    UninitializeStatisticsRecorder();
    StatisticsRecorder::is_vlog_initialized_ = false;
  }
