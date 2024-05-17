TextTrackCue::TextTrackCue(double start, double end)
    : start_time_(start),
      end_time_(end),
      track_(nullptr),
      cue_index_(kInvalidCueIndex),
      is_active_(false),
      pause_on_exit_(false) {}
