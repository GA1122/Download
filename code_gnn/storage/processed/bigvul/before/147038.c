WebPerformance WebLocalFrameImpl::Performance() const {
  if (!GetFrame())
    return WebPerformance();
  return WebPerformance(
      DOMWindowPerformance::performance(*(GetFrame()->DomWindow())));
}
