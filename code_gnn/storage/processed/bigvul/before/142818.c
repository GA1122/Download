bool IsDocumentCrossOrigin(Document& document) {
  const LocalFrame* frame = document.GetFrame();
  return frame && frame->IsCrossOriginSubframe();
}
