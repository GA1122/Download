static bool CueIsBefore(const TextTrackCue* cue, TextTrackCue* other_cue) {
  if (cue->startTime() < other_cue->startTime())
    return true;

  return cue->startTime() == other_cue->startTime() &&
         cue->endTime() > other_cue->endTime();
}
