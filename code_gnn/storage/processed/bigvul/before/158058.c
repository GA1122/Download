void LocalFrameClientImpl::DidObserveNewFeatureUsage(
    mojom::WebFeature feature) {
  if (web_frame_->Client())
    web_frame_->Client()->DidObserveNewFeatureUsage(feature);
}
