void BaseAudioContext::MaybeUnlockUserGesture() {
  if (!user_gesture_required_ || !AreAutoplayRequirementsFulfilled())
    return;

  DCHECK(!autoplay_status_.has_value() ||
         autoplay_status_ != AutoplayStatus::kAutoplayStatusSucceeded);

  user_gesture_required_ = false;
  autoplay_status_ = AutoplayStatus::kAutoplayStatusSucceeded;
}
