bool HTMLMediaElement::isInCrossOriginFrame() const {
  return isDocumentCrossOrigin(document());
}
