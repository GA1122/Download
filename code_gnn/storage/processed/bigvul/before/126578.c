void TabStripGtk::StartMiniMoveTabAnimation(int from_index,
                                            int to_index,
                                            const gfx::Rect& start_bounds) {
  StopAnimation();
  active_animation_.reset(
      new MiniMoveAnimation(this, from_index, to_index, start_bounds));
  active_animation_->Start();
}
