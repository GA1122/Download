InspectorScheduleStyleInvalidationTrackingEvent::IdChange(
    Element& element,
    const InvalidationSet& invalidation_set,
    const AtomicString& id) {
  std::unique_ptr<TracedValue> value =
      FillCommonPart(element, invalidation_set, kId);
  value->SetString("changedId", id);
  return value;
}
