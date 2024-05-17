void TabStripGtk::StartMoveTabAnimation(int from_index, int to_index) {
  StopAnimation();
  active_animation_.reset(new MoveTabAnimation(this, from_index, to_index));
  active_animation_->Start();
}
