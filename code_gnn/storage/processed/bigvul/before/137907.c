void AXLayoutObject::addChildren() {
  ASSERT(!isDetached());
  ASSERT(!m_haveChildren);

  m_haveChildren = true;

  if (!canHaveChildren())
    return;

  HeapVector<Member<AXObject>> ownedChildren;
  computeAriaOwnsChildren(ownedChildren);

  for (AXObject* obj = rawFirstChild(); obj; obj = obj->rawNextSibling()) {
    if (!axObjectCache().isAriaOwned(obj)) {
      obj->setParent(this);
      addChild(obj);
    }
  }

  addHiddenChildren();
  addPopupChildren();
  addImageMapChildren();
  addTextFieldChildren();
  addCanvasChildren();
  addRemoteSVGChildren();
  addInlineTextBoxChildren(false);

  for (const auto& child : m_children) {
    if (!child->cachedParentObject())
      child->setParent(this);
  }

  for (const auto& ownedChild : ownedChildren)
    addChild(ownedChild);
}
