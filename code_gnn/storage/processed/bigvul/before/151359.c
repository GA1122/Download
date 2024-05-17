InspectorStyleInvalidatorInvalidateEvent::InvalidationList(
    ContainerNode& node,
    const Vector<RefPtr<InvalidationSet>>& invalidation_list) {
  std::unique_ptr<TracedValue> value =
      FillCommonPart(node, kElementHasPendingInvalidationList);
  value->BeginArray("invalidationList");
  for (const auto& invalidation_set : invalidation_list)
    invalidation_set->ToTracedValue(value.get());
  value->EndArray();
  return value;
}
