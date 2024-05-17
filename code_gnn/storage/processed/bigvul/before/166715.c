void ThreadHeap::WeakProcessing(Visitor* visitor) {
  TRACE_EVENT0("blink_gc", "ThreadHeap::WeakProcessing");
  double start_time = WTF::CurrentTimeTicksInMilliseconds();

  ThreadState::ObjectResurrectionForbiddenScope object_resurrection_forbidden(
      ThreadState::Current());

  CustomCallbackItem item;
  while (weak_callback_worklist_->Pop(WorklistTaskId::MainThread, &item)) {
    item.callback(visitor, item.object);
  }
  DCHECK(marking_worklist_->IsGlobalEmpty());

  double time_for_weak_processing =
      WTF::CurrentTimeTicksInMilliseconds() - start_time;
  DEFINE_THREAD_SAFE_STATIC_LOCAL(
      CustomCountHistogram, weak_processing_time_histogram,
      ("BlinkGC.TimeForGlobalWeakProcessing", 1, 10 * 1000, 50));
  weak_processing_time_histogram.Count(time_for_weak_processing);
}
