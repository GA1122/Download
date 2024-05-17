void PersistentHistogramAllocator::MergeHistogramDeltaToStatisticsRecorder(
    HistogramBase* histogram) {
  DCHECK(histogram);

  HistogramBase* existing = GetOrCreateStatisticsRecorderHistogram(histogram);
  if (!existing) {
    NOTREACHED();
    return;
  }

  existing->AddSamples(*histogram->SnapshotDelta());
}
