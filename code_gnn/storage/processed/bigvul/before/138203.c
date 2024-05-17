bool AXObject::supportsARIAAttributes() const {
  return isLiveRegion() || supportsARIADragging() || supportsARIADropping() ||
         supportsARIAFlowTo() || supportsARIAOwns() ||
         hasAttribute(aria_labelAttr);
}
