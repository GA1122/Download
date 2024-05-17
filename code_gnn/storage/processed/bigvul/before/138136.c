bool AXObject::computeAncestorExposesActiveDescendant() const {
  const AXObject* parent = parentObjectUnignored();
  if (!parent)
    return false;

  if (parent->supportsActiveDescendant() &&
      !parent->getAttribute(aria_activedescendantAttr).isEmpty()) {
    return true;
  }

  return parent->ancestorExposesActiveDescendant();
}
