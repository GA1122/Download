void RenderFrameImpl::DidObserveLayoutJank(double jank_fraction) {
  for (auto& observer : observers_)
    observer.DidObserveLayoutJank(jank_fraction);
}
