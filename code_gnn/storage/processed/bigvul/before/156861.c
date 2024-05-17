void Document::setBgColor(const AtomicString& value) {
  if (!IsFrameSet())
    SetBodyAttribute(bgcolorAttr, value);
}
