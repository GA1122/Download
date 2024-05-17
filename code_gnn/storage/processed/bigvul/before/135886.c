TextTrack::TextTrack(const AtomicString& kind,
                     const AtomicString& label,
                     const AtomicString& language,
                     const AtomicString& id,
                     TextTrackType type)
    : TrackBase(WebMediaPlayer::kTextTrack, kind, label, language, id),
      active_cues_(nullptr),
      track_list_(nullptr),
      mode_(DisabledKeyword()),
      track_type_(type),
      readiness_state_(kNotLoaded),
      track_index_(kInvalidTrackIndex),
      rendered_track_index_(kInvalidTrackIndex),
      has_been_configured_(false) {}
