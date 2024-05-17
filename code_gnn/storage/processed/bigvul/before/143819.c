  void InitializeStatisticsRecorder() {
    DCHECK(!statistics_recorder_);
    statistics_recorder_ = StatisticsRecorder::CreateTemporaryForTesting();
  }
