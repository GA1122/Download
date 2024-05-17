const DocumentTiming* PerformanceNavigationTiming::GetDocumentTiming() const {
  if (!GetFrame())
    return nullptr;
  Document* document = GetFrame()->GetDocument();
  if (!document)
    return nullptr;

  return &document->GetTiming();
}
