void Document::setFgColor(const AtomicString& value) {
  if (!IsFrameSet())
    SetBodyAttribute(textAttr, value);
}
