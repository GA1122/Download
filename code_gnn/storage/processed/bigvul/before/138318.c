std::unique_ptr<AXNode> InspectorAccessibilityAgent::buildObjectForIgnoredNode(
    Node* domNode,
    AXObject* axObject,
    bool fetchRelatives,
    std::unique_ptr<protocol::Array<AXNode>>& nodes,
    AXObjectCacheImpl& cache) const {
  AXObject::IgnoredReasons ignoredReasons;
  AXID axID = kIDForInspectedNodeWithNoAXNode;
  if (axObject && axObject->isAXLayoutObject())
    axID = axObject->axObjectID();
  std::unique_ptr<AXNode> ignoredNodeObject =
      AXNode::create().setNodeId(String::number(axID)).setIgnored(true).build();
  AccessibilityRole role = AccessibilityRole::IgnoredRole;
  ignoredNodeObject->setRole(createRoleNameValue(role));

  if (axObject && axObject->isAXLayoutObject()) {
    axObject->computeAccessibilityIsIgnored(&ignoredReasons);

    AXObject* parentObject = axObject->parentObjectUnignored();
    if (parentObject && fetchRelatives)
      addAncestors(*parentObject, axObject, nodes, cache);
  } else if (domNode && !domNode->layoutObject()) {
    if (fetchRelatives) {
      populateDOMNodeAncestors(*domNode, *(ignoredNodeObject.get()), nodes,
                               cache);
    }
    ignoredReasons.push_back(IgnoredReason(AXNotRendered));
  }

  if (domNode)
    ignoredNodeObject->setBackendDOMNodeId(DOMNodeIds::idForNode(domNode));

  std::unique_ptr<protocol::Array<AXProperty>> ignoredReasonProperties =
      protocol::Array<AXProperty>::create();
  for (size_t i = 0; i < ignoredReasons.size(); i++)
    ignoredReasonProperties->addItem(createProperty(ignoredReasons[i]));
  ignoredNodeObject->setIgnoredReasons(std::move(ignoredReasonProperties));

  return ignoredNodeObject;
}
