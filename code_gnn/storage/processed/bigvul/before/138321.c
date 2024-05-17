std::unique_ptr<AXProperty> createRelatedNodeListProperty(
    const String& key,
    AXObject::AXObjectVector& nodes,
    const QualifiedName& attr,
    AXObject& axObject) {
  std::unique_ptr<AXValue> nodeListValue = createRelatedNodeListValue(nodes);
  const AtomicString& attrValue = axObject.getAttribute(attr);
  nodeListValue->setValue(protocol::StringValue::create(attrValue));
  return createProperty(key, std::move(nodeListValue));
}
