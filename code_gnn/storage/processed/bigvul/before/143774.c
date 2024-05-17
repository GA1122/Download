  void InitializeStatisticsRecorder() {
    DCHECK(!statistics_recorder_);
    statistics_recorder_ = StatisticsRecorder::CreateTemporaryForTesting();
    auto record_checker = std::make_unique<TestRecordHistogramChecker>();
    StatisticsRecorder::SetRecordChecker(std::move(record_checker));
  }
