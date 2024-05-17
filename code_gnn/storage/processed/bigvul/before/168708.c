DocumentLoader* PerformanceNavigationTiming::GetDocumentLoader() const {
  if (!GetFrame())
    return nullptr;
  return GetFrame()->Loader().GetDocumentLoader();
}
