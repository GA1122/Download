InspectorScheduleStyleInvalidationTrackingEvent::RuleSetInvalidation(
    ContainerNode& root_node,
    const InvalidationSet& invalidation_set) {
  std::unique_ptr<TracedValue> value =
      FillCommonPart(root_node, invalidation_set, kRuleSet);
  return value;
}
