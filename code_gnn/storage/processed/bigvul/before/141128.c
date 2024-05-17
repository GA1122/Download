bool Document::IsFrameSet() const {
  if (!IsHTMLDocument())
    return false;
  return IsHTMLFrameSetElement(body());
}
