void InspectorAccessibilityAgent::populateRelatives(
    AXObject& axObject,
    AXObject* inspectedAXObject,
    AXNode& nodeObject,
    std::unique_ptr<protocol::Array<AXNode>>& nodes,
    AXObjectCacheImpl& cache) const {
  AXObject* parentObject = axObject.parentObject();
  if (parentObject && parentObject != inspectedAXObject) {
    parentObject = axObject.parentObjectUnignored();
  }

  std::unique_ptr<protocol::Array<AXNodeId>> childIds =
      protocol::Array<AXNodeId>::create();

  if (&axObject != inspectedAXObject ||
      (inspectedAXObject && !inspectedAXObject->accessibilityIsIgnored())) {
    addChildren(axObject, inspectedAXObject, childIds, nodes, cache);
  }
  nodeObject.setChildIds(std::move(childIds));
}
