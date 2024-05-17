KURL HTMLInputElement::Src() const {
  return GetDocument().CompleteURL(FastGetAttribute(srcAttr));
}
