void Document::setAlinkColor(const AtomicString& value) {
  if (!IsFrameSet())
    SetBodyAttribute(alinkAttr, value);
}
