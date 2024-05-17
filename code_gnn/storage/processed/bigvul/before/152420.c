void RenderFrameImpl::OnBlinkFeatureUsageReport(const std::set<int>& features) {
  frame_->BlinkFeatureUsageReport(features);
}
