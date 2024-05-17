std::unique_ptr<AXProperty> createRelatedNodeListProperty(
    const String& key,
    AXRelatedObjectVector& nodes) {
  std::unique_ptr<AXValue> nodeListValue =
      createRelatedNodeListValue(nodes, AXValueTypeEnum::NodeList);
  return createProperty(key, std::move(nodeListValue));
}
