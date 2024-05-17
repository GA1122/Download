bool WebMediaPlayerImpl::ShouldPauseVideoWhenHidden() const {
  if (IsBackgroundSuspendEnabled(delegate_)) {
    if (!HasVideo())
      return false;

#if defined(OS_ANDROID)
    if (IsRemote())
      return false;
#endif

    return !HasAudio() || (IsResumeBackgroundVideosEnabled() &&
                           video_locked_when_paused_when_hidden_);
  }

  return IsBackgroundVideoPauseOptimizationEnabled() && !HasAudio() &&
         IsBackgroundOptimizationCandidate();
}
