void InspectorAccessibilityAgent::addAncestors(
    AXObject& firstAncestor,
    AXObject* inspectedAXObject,
    std::unique_ptr<protocol::Array<AXNode>>& nodes,
    AXObjectCacheImpl& cache) const {
  AXObject* ancestor = &firstAncestor;
  while (ancestor) {
    nodes->addItem(buildProtocolAXObject(*ancestor, inspectedAXObject, true,
                                         nodes, cache));
    ancestor = ancestor->parentObjectUnignored();
  }
}
