void Document::DispatchFreezeEvent() {
  DCHECK(RuntimeEnabledFeatures::PageLifecycleEnabled());
  const TimeTicks freeze_event_start = CurrentTimeTicks();
  SetFreezingInProgress(true);
  DispatchEvent(*Event::Create(event_type_names::kFreeze));
  SetFreezingInProgress(false);
  const TimeTicks freeze_event_end = CurrentTimeTicks();
  DEFINE_STATIC_LOCAL(CustomCountHistogram, freeze_histogram,
                      ("DocumentEventTiming.FreezeDuration", 0, 10000000, 50));
  freeze_histogram.CountMicroseconds(freeze_event_end - freeze_event_start);
  UseCounter::Count(*this, WebFeature::kPageLifeCycleFreeze);
}
