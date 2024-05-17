bool LayerTreeHostImpl::AnimateLayers(base::TimeTicks monotonic_time) {
  const bool animated = mutator_host_->TickAnimations(monotonic_time);

  if (animated)
    SetNeedsOneBeginImplFrame();
  return animated;
}
