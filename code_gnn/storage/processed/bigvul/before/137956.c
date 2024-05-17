bool AXLayoutObject::isLinked() const {
  if (!isLinkable(*this))
    return false;

  Element* anchor = anchorElement();
  if (!isHTMLAnchorElement(anchor))
    return false;

  return !toHTMLAnchorElement(*anchor).href().isEmpty();
}
