InspectorScheduleStyleInvalidationTrackingEvent::AttributeChange(
    Element& element,
    const InvalidationSet& invalidation_set,
    const QualifiedName& attribute_name) {
  std::unique_ptr<TracedValue> value =
      FillCommonPart(element, invalidation_set, kAttribute);
  value->SetString("changedAttribute", attribute_name.ToString());
  return value;
}
