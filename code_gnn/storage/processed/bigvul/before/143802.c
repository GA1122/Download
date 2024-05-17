void GlobalHistogramAllocator::ImportHistogramsToStatisticsRecorder() {
  Reference record_to_ignore = last_created();

  while (true) {
    std::unique_ptr<HistogramBase> histogram =
        import_iterator_.GetNextWithIgnore(record_to_ignore);
    if (!histogram)
      break;
    StatisticsRecorder::RegisterOrDeleteDuplicate(histogram.release());
  }
}
