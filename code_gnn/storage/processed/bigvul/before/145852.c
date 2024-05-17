  void AttemptReveal(Modality modality) {
    ASSERT_NE(modality, MODALITY_GESTURE_TAP);
    AttemptRevealStateChange(true, modality);
  }
