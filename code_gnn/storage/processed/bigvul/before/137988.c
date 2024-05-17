bool AXLayoutObject::supportsARIAFlowTo() const {
  return !getAttribute(aria_flowtoAttr).isEmpty();
}
