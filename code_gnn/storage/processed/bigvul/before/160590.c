void RenderFrameImpl::DidMeaningfulLayout(
    blink::WebMeaningfulLayout layout_type) {
  SCOPED_UMA_HISTOGRAM_TIMER("RenderFrameObservers.DidMeaningfulLayout");
  for (auto& observer : observers_)
    observer.DidMeaningfulLayout(layout_type);
}
