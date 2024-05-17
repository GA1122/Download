void InspectorAccessibilityAgent::fillCoreProperties(
    AXObject& axObject,
    AXObject* inspectedAXObject,
    bool fetchRelatives,
    AXNode& nodeObject,
    std::unique_ptr<protocol::Array<AXNode>>& nodes,
    AXObjectCacheImpl& cache) const {
  AXNameFrom nameFrom;
  AXObject::AXObjectVector nameObjects;
  axObject.name(nameFrom, &nameObjects);

  AXDescriptionFrom descriptionFrom;
  AXObject::AXObjectVector descriptionObjects;
  String description =
      axObject.description(nameFrom, descriptionFrom, &descriptionObjects);
  if (!description.isEmpty()) {
    nodeObject.setDescription(
        createValue(description, AXValueTypeEnum::ComputedString));
  }
  if (axObject.supportsRangeValue()) {
    nodeObject.setValue(createValue(axObject.valueForRange()));
  } else {
    String stringValue = axObject.stringValue();
    if (!stringValue.isEmpty())
      nodeObject.setValue(createValue(stringValue));
  }

  if (fetchRelatives)
    populateRelatives(axObject, inspectedAXObject, nodeObject, nodes, cache);

  Node* node = axObject.getNode();
  if (node)
    nodeObject.setBackendDOMNodeId(DOMNodeIds::idForNode(node));
}
