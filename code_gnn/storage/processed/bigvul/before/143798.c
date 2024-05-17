PersistentHistogramAllocator::GetOrCreateStatisticsRecorderHistogram(
    const HistogramBase* histogram) {
  DCHECK_NE(GlobalHistogramAllocator::Get(), this);
  DCHECK(histogram);

  HistogramBase* existing =
      StatisticsRecorder::FindHistogram(histogram->histogram_name());
  if (existing)
    return existing;

  base::Pickle pickle;
  histogram->SerializeInfo(&pickle);
  PickleIterator iter(pickle);
  existing = DeserializeHistogramInfo(&iter);
  if (!existing)
    return nullptr;

  DCHECK_EQ(0, existing->flags() & HistogramBase::kIPCSerializationSourceFlag);
   return StatisticsRecorder::RegisterOrDeleteDuplicate(existing);
 }
