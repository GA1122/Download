void LayerTreeHost::UpdateClientAnimations(base::TimeTicks frame_begin_time) {
  animating_ = true;
  client_->Animate(frame_begin_time);
  animating_ = false;
}
