bool isDocumentCrossOrigin(Document& document) {
  const LocalFrame* frame = document.frame();
  return frame && frame->isCrossOriginSubframe();
}
