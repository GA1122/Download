bool Document::HasSVGRootNode() const {
  return isSVGSVGElement(documentElement());
}
