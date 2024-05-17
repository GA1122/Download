void Performance::AddLongTaskTiming(
    TimeTicks start_time,
    TimeTicks end_time,
    const String& name,
    const String& frame_src,
    const String& frame_id,
    const String& frame_name,
    const SubTaskAttribution::EntriesVector& sub_task_attributions) {
  if (!HasObserverFor(PerformanceEntry::kLongTask))
    return;

  for (auto&& it : sub_task_attributions) {
    it->setHighResStartTime(
        MonotonicTimeToDOMHighResTimeStamp(it->startTime()));
    it->setHighResDuration(
        ConvertTimeDeltaToDOMHighResTimeStamp(it->duration()));
  }
  PerformanceEntry* entry = PerformanceLongTaskTiming::Create(
      MonotonicTimeToDOMHighResTimeStamp(start_time),
      MonotonicTimeToDOMHighResTimeStamp(end_time), name, frame_src, frame_id,
      frame_name, sub_task_attributions);
  NotifyObserversOfEntry(*entry);
}
