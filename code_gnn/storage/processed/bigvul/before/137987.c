 bool AXLayoutObject::supportsARIADropping() const {
  const AtomicString& dropEffect = getAttribute(aria_dropeffectAttr);
  return !dropEffect.isEmpty();
}
