void Document::setLinkColor(const AtomicString& value) {
  if (!IsFrameSet())
    SetBodyAttribute(linkAttr, value);
}
