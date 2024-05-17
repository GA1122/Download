void HTMLMediaElement::SetOfficialPlaybackPosition(double position) const {
#if LOG_OFFICIAL_TIME_STATUS
  BLINK_MEDIA_LOG << "SetOfficialPlaybackPosition(" << (void*)this
                  << ") was:" << official_playback_position_
                  << " now:" << position;
#endif

  official_playback_position_ =
      std::isnan(duration()) ? position : std::min(duration(), position);

  if (official_playback_position_ != position) {
    BLINK_MEDIA_LOG << "setOfficialPlaybackPosition(" << (void*)this
                    << ") position:" << position
                    << " truncated to duration:" << official_playback_position_;
  }

  official_playback_position_needs_update_ = false;
  Microtask::EnqueueMicrotask(
      WTF::Bind(&HTMLMediaElement::RequireOfficialPlaybackPositionUpdate,
                WrapWeakPersistent(this)));
}
