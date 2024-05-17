void InspectorAccessibilityAgent::populateDOMNodeAncestors(
    Node& inspectedDOMNode,
    AXNode& nodeObject,
    std::unique_ptr<protocol::Array<AXNode>>& nodes,
    AXObjectCacheImpl& cache) const {
  Node* parentNode = inspectedDOMNode.isShadowRoot()
                         ? &toShadowRoot(inspectedDOMNode).host()
                         : FlatTreeTraversal::parent(inspectedDOMNode);
  AXObject* parentAXObject = cache.getOrCreate(parentNode);
  while (parentNode && !parentAXObject) {
    parentNode = parentNode->isShadowRoot()
                     ? &toShadowRoot(parentNode)->host()
                     : FlatTreeTraversal::parent(*parentNode);
    parentAXObject = cache.getOrCreate(parentNode);
  }

  if (!parentAXObject)
    return;

  if (parentAXObject->accessibilityIsIgnored())
    parentAXObject = parentAXObject->parentObjectUnignored();
  if (!parentAXObject)
    return;

  std::unique_ptr<AXNode> parentNodeObject =
      buildProtocolAXObject(*parentAXObject, nullptr, true, nodes, cache);
  std::unique_ptr<protocol::Array<AXNodeId>> childIds =
      protocol::Array<AXNodeId>::create();
  childIds->addItem(String::number(kIDForInspectedNodeWithNoAXNode));
  parentNodeObject->setChildIds(std::move(childIds));
  nodes->addItem(std::move(parentNodeObject));

  AXObject* grandparentAXObject = parentAXObject->parentObjectUnignored();
  if (grandparentAXObject)
    addAncestors(*grandparentAXObject, nullptr, nodes, cache);
}
