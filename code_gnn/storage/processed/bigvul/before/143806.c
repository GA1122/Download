void PersistentHistogramAllocator::MergeHistogramFinalDeltaToStatisticsRecorder(
    const HistogramBase* histogram) {
  DCHECK(histogram);

  HistogramBase* existing = GetOrCreateStatisticsRecorderHistogram(histogram);
  if (!existing) {
    NOTREACHED();
    return;
  }

  existing->AddSamples(*histogram->SnapshotFinalDelta());
}
