Response InspectorAccessibilityAgent::getPartialAXTree(
    int domNodeId,
    Maybe<bool> fetchRelatives,
    std::unique_ptr<protocol::Array<AXNode>>* nodes) {
  if (!m_domAgent->enabled())
    return Response::Error("DOM agent must be enabled");
  Node* domNode = nullptr;
  Response response = m_domAgent->assertNode(domNodeId, domNode);
  if (!response.isSuccess())
    return response;

  Document& document = domNode->document();
  document.updateStyleAndLayoutIgnorePendingStylesheets();
  DocumentLifecycle::DisallowTransitionScope disallowTransition(
      document.lifecycle());
  LocalFrame* localFrame = document.frame();
  if (!localFrame)
    return Response::Error("Frame is detached.");
  std::unique_ptr<ScopedAXObjectCache> scopedCache =
      ScopedAXObjectCache::create(document);
  AXObjectCacheImpl* cache = toAXObjectCacheImpl(scopedCache->get());

  AXObject* inspectedAXObject = cache->getOrCreate(domNode);
  *nodes = protocol::Array<protocol::Accessibility::AXNode>::create();
  if (!inspectedAXObject || inspectedAXObject->accessibilityIsIgnored()) {
    (*nodes)->addItem(buildObjectForIgnoredNode(domNode, inspectedAXObject,
                                                fetchRelatives.fromMaybe(true),
                                                *nodes, *cache));
    return Response::OK();
  } else {
    (*nodes)->addItem(
        buildProtocolAXObject(*inspectedAXObject, inspectedAXObject,
                              fetchRelatives.fromMaybe(true), *nodes, *cache));
  }

  if (!inspectedAXObject)
    return Response::OK();

  AXObject* parent = inspectedAXObject->parentObjectUnignored();
  if (!parent)
    return Response::OK();

  if (fetchRelatives.fromMaybe(true))
    addAncestors(*parent, inspectedAXObject, *nodes, *cache);

  return Response::OK();
}
