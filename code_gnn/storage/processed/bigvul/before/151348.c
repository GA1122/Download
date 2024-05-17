std::unique_ptr<TracedValue> FillCommonPart(
    ContainerNode& node,
    const InvalidationSet& invalidation_set,
    const char* invalidated_selector) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("frame", ToHexString(node.GetDocument().GetFrame()));
  SetNodeInfo(value.get(), &node, "nodeId", "nodeName");
  value->SetString("invalidationSet",
                   DescendantInvalidationSetToIdString(invalidation_set));
  value->SetString("invalidatedSelectorId", invalidated_selector);
  SourceLocation::Capture()->ToTracedValue(value.get(), "stackTrace");
  return value;
}
