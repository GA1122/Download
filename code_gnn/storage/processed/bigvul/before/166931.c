void BaseAudioContext::MaybeRecordStartAttempt() {
  if (!user_gesture_required_ || !AreAutoplayRequirementsFulfilled())
    return;

  DCHECK(!autoplay_status_.has_value() ||
         autoplay_status_ != AutoplayStatus::kAutoplayStatusSucceeded);
  autoplay_status_ = AutoplayStatus::kAutoplayStatusFailedWithStart;
}
