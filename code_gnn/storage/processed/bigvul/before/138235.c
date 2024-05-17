AXObject* AXObjectCacheImpl::get(Node* node) {
  if (!node)
    return 0;

  LayoutObject* layoutObject = node->layoutObject();
  if (isMenuListOption(node) || isHTMLAreaElement(node))
    layoutObject = nullptr;

  AXID layoutID = layoutObject ? m_layoutObjectMapping.at(layoutObject) : 0;
  DCHECK(!HashTraits<AXID>::isDeletedValue(layoutID));

  AXID nodeID = m_nodeObjectMapping.at(node);
  DCHECK(!HashTraits<AXID>::isDeletedValue(nodeID));

  if (layoutObject && nodeID && !layoutID) {
    remove(nodeID);
    return 0;
  }

  if (layoutID)
    return m_objects.at(layoutID);

  if (!nodeID)
    return 0;

  return m_objects.at(nodeID);
}
