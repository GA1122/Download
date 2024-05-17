GestureSequence::GestureSequence()
    : state_(GS_NO_GESTURE),
      flags_(0),
      pinch_distance_start_(0.f),
      pinch_distance_current_(0.f),
      point_count_(0) {
}
