void RenderFrameImpl::DidChangeActiveSchedulerTrackedFeatures(
    uint64_t features_mask) {
  GetFrameHost()->UpdateActiveSchedulerTrackedFeatures(features_mask);
}
