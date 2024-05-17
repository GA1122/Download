void CueTimeline::AddCueInternal(TextTrackCue* cue) {
  double end_time = std::max(cue->startTime(), cue->endTime());

  CueInterval interval =
      cue_tree_.CreateInterval(cue->startTime(), end_time, cue);
  if (!cue_tree_.Contains(interval))
    cue_tree_.Add(interval);
}
