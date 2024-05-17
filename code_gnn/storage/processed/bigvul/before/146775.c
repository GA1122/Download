bool Document::IsFrameSet() const {
  if (!IsHTMLDocument())
    return false;
  return isHTMLFrameSetElement(body());
}
