AtomicString PerformanceNavigationTiming::type() const {
  DocumentLoader* loader = GetDocumentLoader();
  if (GetFrame() && loader)
    return GetNavigationType(loader->GetNavigationType(),
                             GetFrame()->GetDocument());
  return "navigate";
}
