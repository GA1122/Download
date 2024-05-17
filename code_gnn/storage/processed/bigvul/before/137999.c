KURL AXLayoutObject::url() const {
  if (isAnchor() && isHTMLAnchorElement(m_layoutObject->node())) {
    if (HTMLAnchorElement* anchor = toHTMLAnchorElement(anchorElement()))
      return anchor->href();
  }

  if (isWebArea())
    return m_layoutObject->document().url();

  if (isImage() && isHTMLImageElement(m_layoutObject->node()))
    return toHTMLImageElement(*m_layoutObject->node()).src();

  if (isInputImage())
    return toHTMLInputElement(m_layoutObject->node())->src();

  return KURL();
}
