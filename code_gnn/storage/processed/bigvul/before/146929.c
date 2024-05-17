void WebLocalFrameImpl::BlinkFeatureUsageReport(const std::set<int>& features) {
  DCHECK(!features.empty());
  for (int feature : features) {
    UseCounter::Count(GetFrame(), static_cast<WebFeature>(feature));
  }
}
