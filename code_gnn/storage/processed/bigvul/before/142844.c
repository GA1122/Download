double HTMLMediaElement::OfficialPlaybackPosition() const {
  bool waiting_for_data = ready_state_ <= kHaveCurrentData;
  if (official_playback_position_needs_update_ && !paused_ &&
      !waiting_for_data) {
    SetOfficialPlaybackPosition(CurrentPlaybackPosition());
  }

#if LOG_OFFICIAL_TIME_STATUS
  static const double kMinCachedDeltaForWarning = 0.01;
  double delta =
      std::abs(official_playback_position_ - CurrentPlaybackPosition());
  if (delta > kMinCachedDeltaForWarning) {
    BLINK_MEDIA_LOG << "CurrentTime(" << (void*)this
                    << ") - WARNING, cached time is " << delta
                    << "seconds off of media time when paused/waiting";
  }
#endif

  return official_playback_position_;
}
