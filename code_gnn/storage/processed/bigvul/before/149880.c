void LayerTreeHostImpl::ActivateAnimations() {
  const bool activated = mutator_host_->ActivateAnimations();
  if (activated) {
    active_tree()->set_needs_update_draw_properties();
    SetNeedsOneBeginImplFrame();
  }
}
