void PerformanceNavigationTiming::Trace(blink::Visitor* visitor) {
  ContextClient::Trace(visitor);
  PerformanceResourceTiming::Trace(visitor);
}
