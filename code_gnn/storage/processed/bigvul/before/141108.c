bool Document::HasSVGRootNode() const {
  return IsSVGSVGElement(documentElement());
}
