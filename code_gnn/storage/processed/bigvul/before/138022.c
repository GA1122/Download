void AXNodeObject::addChildren() {
  ASSERT(!isDetached());
  ASSERT(!m_haveChildren);

  if (!m_node)
    return;

  m_haveChildren = true;

  if (getLayoutObject() && !isHTMLCanvasElement(*m_node))
    return;

  HeapVector<Member<AXObject>> ownedChildren;
  computeAriaOwnsChildren(ownedChildren);

  for (Node& child : NodeTraversal::childrenOf(*m_node)) {
    AXObject* childObj = axObjectCache().getOrCreate(&child);
    if (childObj && !axObjectCache().isAriaOwned(childObj))
      addChild(childObj);
  }

  for (const auto& ownedChild : ownedChildren)
    addChild(ownedChild);

  for (const auto& child : m_children)
    child->setParent(this);
}
