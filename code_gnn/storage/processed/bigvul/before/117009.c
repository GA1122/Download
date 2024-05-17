void GestureSequence::Reset() {
  set_state(GS_NO_GESTURE);
  for (int i = 0; i < point_count_; ++i)
    points_[i].Reset();
}
