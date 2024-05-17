LocalFrame::~LocalFrame() {
  DCHECK(!view_);
  if (is_ad_subframe_)
    InstanceCounters::DecrementCounter(InstanceCounters::kAdSubframeCounter);
}
