void TabStripGtk::StartInsertTabAnimation(int index) {
  available_width_for_tabs_ = -1;
  StopAnimation();
  active_animation_.reset(new InsertTabAnimation(this, index));
  active_animation_->Start();
}
