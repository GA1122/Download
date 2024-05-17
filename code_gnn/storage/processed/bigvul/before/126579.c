void TabStripGtk::StartMiniTabAnimation(int index) {
  StopAnimation();
  active_animation_.reset(new MiniTabAnimation(this, index));
  active_animation_->Start();
}
