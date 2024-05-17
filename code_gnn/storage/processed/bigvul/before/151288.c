InspectorScheduleStyleInvalidationTrackingEvent::ClassChange(
    Element& element,
    const InvalidationSet& invalidation_set,
    const AtomicString& class_name) {
  std::unique_ptr<TracedValue> value =
      FillCommonPart(element, invalidation_set, kClass);
  value->SetString("changedClass", class_name);
  return value;
}
