void Document::setVlinkColor(const AtomicString& value) {
  if (!IsFrameSet())
    SetBodyAttribute(vlinkAttr, value);
}
