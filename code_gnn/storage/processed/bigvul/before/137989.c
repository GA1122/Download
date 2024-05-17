bool AXLayoutObject::supportsARIAOwns() const {
  if (!m_layoutObject)
    return false;
  const AtomicString& ariaOwns = getAttribute(aria_ownsAttr);

  return !ariaOwns.isEmpty();
}
