AXObject* AXNodeObject::inPageLinkTarget() const {
  if (!m_node || !isHTMLAnchorElement(m_node) || !getDocument())
    return AXObject::inPageLinkTarget();

  HTMLAnchorElement* anchor = toHTMLAnchorElement(m_node);
  DCHECK(anchor);
  KURL linkURL = anchor->href();
  if (!linkURL.isValid())
    return AXObject::inPageLinkTarget();
  String fragment = linkURL.fragmentIdentifier();
  if (fragment.isEmpty())
    return AXObject::inPageLinkTarget();

  KURL documentURL = getDocument()->url();
  if (!documentURL.isValid() ||
      !equalIgnoringFragmentIdentifier(documentURL, linkURL)) {
    return AXObject::inPageLinkTarget();
  }

  TreeScope& treeScope = anchor->treeScope();
  Element* target = treeScope.findAnchor(fragment);
  if (!target)
    return AXObject::inPageLinkTarget();
  return axObjectCache().firstAccessibleObjectFromNode(target);
}
