bool HTMLMediaElement::IsInCrossOriginFrame() const {
  return IsDocumentCrossOrigin(GetDocument());
}
