void AXNodeObject::computeAriaOwnsChildren(
    HeapVector<Member<AXObject>>& ownedChildren) const {
  if (!hasAttribute(aria_ownsAttr))
    return;

  Vector<String> idVector;
  if (canHaveChildren() && !isNativeTextControl() &&
      !hasContentEditableAttributeSet())
    tokenVectorFromAttribute(idVector, aria_ownsAttr);

  axObjectCache().updateAriaOwns(this, idVector, ownedChildren);
}
