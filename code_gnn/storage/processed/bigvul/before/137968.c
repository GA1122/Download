bool AXLayoutObject::liveRegionAtomic() const {
  if (getAttribute(aria_atomicAttr).isEmpty() &&
      (roleValue() == AlertRole || roleValue() == StatusRole)) {
    return true;
  }
  return elementAttributeValue(aria_atomicAttr);
}
